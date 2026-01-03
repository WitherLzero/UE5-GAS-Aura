// Copyright rynnli


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/CoreAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCoreAttributeSet* PlayerAttributeSet = CastChecked<UCoreAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(PlayerAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PlayerAttributeSet->GetMaxHealth());
}
