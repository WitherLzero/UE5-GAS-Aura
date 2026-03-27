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
#include "GameplayMechanics/Core/Interaction/CombatInterface.h"

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

void URPGAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const FVector& Origin, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets)
{
	OutClosestTargets = Actors;
	if (OutClosestTargets.Num() <= MaxTargets)
	{
		return;
	}
	OutClosestTargets.Sort([&Origin](const AActor& A, const AActor& B)
	{
		const double DistSqA = (A.GetActorLocation() - Origin).SquaredLength();
		const double DistSqB = (B.GetActorLocation() - Origin).SquaredLength();

		return DistSqA < DistSqB;
	});

	OutClosestTargets.SetNum(MaxTargets);
}

bool URPGAbilitySystemLibrary::TraceAttackTrajectory(AActor* Instigator, const FGameplayTag& StartSocketTag,
	const FVector& TargetLocation, float TraceRadius, ETraceTypeQuery TraceChannel,FHitResult& OutHitResult)
{
	if (!Instigator || !Instigator->Implements<UCombatInterface>())
	{
		return false;
	}
	
	const FVector StartLocation = GetCombatSocketLocation(Instigator, StartSocketTag);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Instigator); 
	
	return UKismetSystemLibrary::SphereTraceSingle(
		Instigator,             
		StartLocation, 
		TargetLocation, 
		TraceRadius, 
		TraceChannel, 
		false,                  
		ActorsToIgnore, 
		EDrawDebugTrace::None,  
		OutHitResult, 
		true                   
	);
}


ARPGProjectile* URPGAbilitySystemLibrary::SpawnProjectileTowardsTarget(const UObject* WorldContextObject,
                                                                       TSubclassOf<ARPGProjectile> ProjectileClass, const FVector& SpawnLocation, const FVector& TargetLocation,
                                                                       bool bOverridePitch, float PitchOverride, AActor* Instigator, const FDamageEffectParams& DamageEffectParams)
{
	if (!WorldContextObject || !ProjectileClass) return nullptr;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	if (!Instigator->HasAuthority()) return nullptr;
	
	FRotator Rotation = (TargetLocation - SpawnLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
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

ARPGProjectile* URPGAbilitySystemLibrary::SpawnProjectileInDirection(const UObject* WorldContextObject,
	TSubclassOf<ARPGProjectile> ProjectileClass, const FVector& SpawnLocation, const FRotator& SpawnRotation,
	bool bOverridePitch, float PitchOverride, AActor* Instigator, const FDamageEffectParams& DamageEffectParams)
{
	if (!WorldContextObject || !ProjectileClass) return nullptr;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	if (!Instigator->HasAuthority()) return nullptr;
	
	FRotator NewRotation = SpawnRotation;
	if (bOverridePitch) NewRotation.Pitch = PitchOverride;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(NewRotation.Quaternion());
	
	ARPGProjectile* Projectile = World->SpawnActorDeferred<ARPGProjectile>(
	ProjectileClass,
	SpawnTransform,
	Instigator,Cast<APawn>(Instigator),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = DamageEffectParams;
	
	Projectile->FinishSpawning(SpawnTransform);
	
	return Projectile;
}

TArray<FRotator> URPGAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis( -Spread/2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis( DeltaSpread*i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> URPGAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis( -Spread/2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis( DeltaSpread*i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

FGameplayEffectSpecHandle URPGAbilitySystemLibrary::MakeDamageEffectSpec(const FDamageEffectParams& DamageEffectParams)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	SetDebuffCarrier(EffectContexthandle, DamageEffectParams.DebuffCarrierClass);
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

TSubclassOf<UGameplayEffect> URPGAbilitySystemLibrary::GetDebuffCarrier(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRPGGameplayEffectContext* EffectContext = static_cast<const FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffCarrier();
	}
	return nullptr;
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

void URPGAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInSuccessfulDebuff)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void URPGAbilitySystemLibrary::SetDebuffCarrier(FGameplayEffectContextHandle& EffectContextHandle,
	TSubclassOf<UGameplayEffect> DebuffCarrierClass)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffCarrier(DebuffCarrierClass);
	}
}

void URPGAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffDamage( InDamage);
	}
}

void URPGAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
	float InDuration)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffDuration(InDuration);
	}
}

void URPGAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float InFrequency)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffFrequency(InFrequency);
	}
}

void URPGAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		EffectContext->SetDamageType(DamageType);
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

USkeletalMeshComponent* URPGAbilitySystemLibrary::GetWeapon(const AActor* CombatActor)
{
	if (CombatActor && CombatActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetWeapon(CombatActor);
	}
	return nullptr;
}

FVector URPGAbilitySystemLibrary::GetCombatSocketLocation(const AActor* CombatActor, const FGameplayTag& MontageTag)
{
	if (CombatActor && CombatActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetCombatSocketLocation(CombatActor, MontageTag);
	}
	return FVector::ZeroVector;
}

TArray<FTaggedMontage> URPGAbilitySystemLibrary::GetAttackMontages(const AActor* CombatActor)
{
	if (CombatActor && CombatActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetAttackMontages(CombatActor);
	}
	return TArray<FTaggedMontage>();
}

UAnimMontage* URPGAbilitySystemLibrary::GetHitReactMontage(const AActor* CombatActor)
{
	if (CombatActor && CombatActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetHitReactMontage(CombatActor);
	}
	return nullptr;
}

UNiagaraSystem* URPGAbilitySystemLibrary::GetHitReactEffect(const AActor* CombatActor)
{
	if (CombatActor && CombatActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetHitReactEffect(CombatActor);
	}
	return nullptr;
}

AActor* URPGAbilitySystemLibrary::GetCombatTarget(const AActor* CombatActor)
{
	if (CombatActor && CombatActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetCombatTarget(CombatActor);
	}
	return nullptr;
}
