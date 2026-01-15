// Copyright rynnli


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/UserWidgetBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		
		OverlayWidgetController->BindCallbacksToDependencies();
	
		return OverlayWidgetController;
	}
	
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(UAbilitySystemComponent* ASC, APlayerController* PC, APlayerState* PS)
{
	checkf(OverlayWidgetClass,TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass,TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));
	
	OverlayWidget = CreateWidget<UUserWidgetBase>(GetWorld(), OverlayWidgetClass);
	
	const FWidgetControllerParams WidgetControllerParams(ASC,PC,PS);
	OverlayWidget->SetWidgetController(GetOverlayWidgetController(WidgetControllerParams));
	
	OverlayWidgetController->BroadcastInitialValues();
	
	OverlayWidget->AddToViewport();

}
