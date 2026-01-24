// Copyright rynnli


#include "AbilitySystem/RPGAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Framework/CoreGameModeBase.h"
#include "Framework/CorePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

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
	ACoreGameModeBase* GameMode = Cast<ACoreGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!GameMode) return;
	
	AActor* Avatar = ASC->GetAvatarActor();
	UCharacterClassInfo* CharacterClassInfo = GameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	ApplyEffectToSelf(ASC, Avatar, ClassDefaultInfo.PrimaryAttributes, Level);
	ApplyEffectToSelf(ASC, Avatar, CharacterClassInfo->SecondaryAttributes, Level);
	ApplyEffectToSelf(ASC, Avatar, CharacterClassInfo->VitalAttributes, Level);
}

void URPGAbilitySystemLibrary::ApplyEffectToSelf(UAbilitySystemComponent* ASC, AActor* Avatar, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	FGameplayEffectContextHandle ContextHandle =ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Avatar);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass,Level,ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}