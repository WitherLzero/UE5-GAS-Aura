// Copyright rynnli


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
}
