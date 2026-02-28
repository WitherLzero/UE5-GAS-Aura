// Copyright rynnli


#include "AuraGame/UI/WidgetController/AuraWidgetController.h"

#include "AuraGame/Character/Player/AuraPlayerState.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/Player/RPGPlayerController.h"

URPGAbilitySystemComponent* UAuraWidgetController::GetASC()
{
	if (RPGAbilitySystemComponent == nullptr)
	{
		return CastChecked<URPGAbilitySystemComponent>(AbilitySystemComponent);
	}
	return RPGAbilitySystemComponent;
}

ARPGPlayerController* UAuraWidgetController::GetPC()
{
	if (RPGPlayerController == nullptr)
	{
		return CastChecked<ARPGPlayerController>(PlayerController);
	}
	return RPGPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		return CastChecked<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}
