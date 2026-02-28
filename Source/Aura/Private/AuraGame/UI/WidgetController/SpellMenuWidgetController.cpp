// Copyright rynnli


#include "AuraGame/UI/WidgetController/SpellMenuWidgetController.h"

#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/Data/AbilityInfo.h"


void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnInitializeStartupAbilities(Cast<URPGAbilitySystemComponent>(AbilitySystemComponent));
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if (AbilityInfo)
			{
				FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				OnAbilityInfoGet.Broadcast(Info);
			}
		});
}
