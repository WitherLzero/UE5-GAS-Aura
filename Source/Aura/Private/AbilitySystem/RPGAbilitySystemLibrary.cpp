// Copyright rynnli


#include "AbilitySystem/RPGAbilitySystemLibrary.h"

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
