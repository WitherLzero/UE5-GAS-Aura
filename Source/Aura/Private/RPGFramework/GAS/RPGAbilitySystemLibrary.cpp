// Copyright rynnli


#include "GameplayMechanics/Core/RPGAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayMechanics/Core/Actor/RPGProjectile.h"
#include "RPGFramework/GAS/RPGAbilityTypes.h"
#include "AuraGame/Game/AuraGameMode.h"
#include "RPGFramework/Player/RPGPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "AuraGame/UI/HUD/AuraHUD.h"
#include "AuraGame/GAS/Data/CharacterClassInfo.h"
#include "AuraGame/Types/AuraGameplayTags.h"
#include "RPGFramework/Interaction/CharacterDataInterface.h"
#include "RPGFramework/UI/WidgetController/WidgetController.h"
#include "GameplayMechanics/Core/Components/CombatComponent.h"
#include "GameplayMechanics/Core/Components/VitalityComponent.h"


void URPGAbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
                                                     ECharacterClass CharacterClass, float Level)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	AActor* Avatar = ASC->GetAvatarActor();
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	ApplyEffectToSelf(ASC, Avatar, ClassDefaultInfo.PrimaryAttributes, Level);
	ApplyEffectToSelf(ASC, Avatar, CharacterClassInfo->SecondaryAttributes, Level);
	ApplyEffectToSelf(ASC, Avatar, CharacterClassInfo->VitalAttributes, Level);
}


void URPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility> AbilityClass : ClassDefaultInfo.StartupAbilities)
	{
		if (ICharacterDataInterface* CharacterData =  Cast<ICharacterDataInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,CharacterData->GetCharacterLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
	
}

int32 URPGAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
	
	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* URPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameMode* GameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GameMode) return nullptr;
	
	return GameMode->CharacterClassInfo;
}

UAbilityInfo* URPGAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	ARPGGameModeBase* GameMode = Cast<ARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GameMode) return nullptr;
	
	return GameMode->AbilityInfo;
}

void URPGAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                          TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                          const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, 
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (UVitalityComponent* VitalityComp =  Overlap.GetActor()->FindComponentByClass<UVitalityComponent>())
			{
				if (!VitalityComp->IsDead())
				{
					OutOverlappingActors.AddUnique(Overlap.GetActor());
				}
			}
		}
	}
}

ARPGProjectile* URPGAbilitySystemLibrary::SpawnProjectileWithDamage(const UObject* WorldContextObject,
	TSubclassOf<ARPGProjectile> ProjectileClass, const FVector& SpawnLocation, const FVector& TargetLocation,
	AActor* Instigator, const FDamageEffectParams& DamageEffectParams)
{
	if (!WorldContextObject || !ProjectileClass) return nullptr;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	if (!Instigator->HasAuthority()) return nullptr;
	
	const FRotator Rotation = (TargetLocation - SpawnLocation).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	ARPGProjectile* Projectile = World->SpawnActorDeferred<ARPGProjectile>(
	ProjectileClass,
	SpawnTransform,
	Instigator,Cast<APawn>(Instigator),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = DamageEffectParams;
	
	Projectile->FinishSpawning(SpawnTransform);
	
	return Projectile;
}

FGameplayEffectSpecHandle URPGAbilitySystemLibrary::MakeDamageEffectSpec(const FDamageEffectParams& DamageEffectParams)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	for (auto& Pair : DamageEffectParams.DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(DamageEffectParams.AbilityLevel);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaledDamage);
	}
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	return SpecHandle;
}


bool URPGAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsBlockedHit();
	}
	return false;
}

bool URPGAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsCriticalHit();
	}
	return false;
}

bool URPGAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float URPGAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float URPGAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float URPGAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag URPGAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (EffectContext->GetDamageType().IsValid())
		{
			return *EffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

void URPGAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void URPGAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

FTaggedMontage URPGAbilitySystemLibrary::PickRandomTaggedMontage(const TArray<FTaggedMontage>& Montages)
{
	const int32 Num = Montages.Num();
	if (Num <= 0) return FTaggedMontage();
	const int32 Index = FMath::RandRange(0,Num - 1);
	return Montages[Index];
}


void URPGAbilitySystemLibrary::ApplyEffectToSelf(UAbilitySystemComponent* ASC, AActor* Avatar, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	FGameplayEffectContextHandle ContextHandle =ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Avatar);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass,Level,ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
