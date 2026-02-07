// Copyright rynnli


#include "RPGFramework/GAS/RPGAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "RPGFramework/GAS/RPGAbilityTypes.h"
#include "AuraGame/Game/AuraGameMode.h"
#include "RPGFramework/Player/CorePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "AuraGame/UI/HUD/AuraHUD.h"
#include "AuraGame/GAS/Data/CharacterClassInfo.h"
#include "RPGFramework/Interaction/CharacterDataInterface.h"
#include "RPGFramework/UI/WidgetController/WidgetController.h"

UAttributeMenuWidgetController* URPGAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			ACorePlayerState* PS = PC->GetPlayerState<ACorePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			
			const FWidgetControllerParams WidgetControllerParams(ASC,PC,PS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	
	return nullptr;
}



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

UCharacterClassInfo* URPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameMode* GameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GameMode) return nullptr;
	
	return GameMode->CharacterClassInfo;
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


void URPGAbilitySystemLibrary::ApplyEffectToSelf(UAbilitySystemComponent* ASC, AActor* Avatar, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	FGameplayEffectContextHandle ContextHandle =ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Avatar);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass,Level,ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
