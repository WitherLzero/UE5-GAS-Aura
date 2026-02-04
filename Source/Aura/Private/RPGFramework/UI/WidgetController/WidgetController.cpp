// Copyright rynnli


#include "RPGFramework/UI/WidgetController/WidgetController.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
}

void UWidgetController::BroadcastInitialValues()
{
}

void UWidgetController::BindCallbacksToDependencies()
{
}
