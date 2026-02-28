// Copyright rynnli


#include "AuraGame/UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGame/Character/Player/AuraPlayerState.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/Data/AbilityInfo.h"




void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnInitializeStartupAbilities(Cast<URPGAbilitySystemComponent>(AbilitySystemComponent));
	
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	SpellPointsChanged.Broadcast(AuraPlayerState->GetSpellPoints());
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
	
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnSpellPointsChanged.AddLambda(
		[this](int32 NewPoints)
		{
			SpellPointsChanged.Broadcast(NewPoints);
		});
}
