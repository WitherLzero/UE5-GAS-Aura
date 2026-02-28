// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/UI/WidgetController/WidgetController.h"
#include "AuraWidgetController.generated.h"

class URPGAbilitySystemComponent;
class ARPGPlayerController;
class AAuraPlayerState;
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<URPGAbilitySystemComponent> RPGAbilitySystemComponent;
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ARPGPlayerController> RPGPlayerController;
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	
public:
	URPGAbilitySystemComponent* GetASC();
	ARPGPlayerController* GetPC();
	AAuraPlayerState* GetAuraPS();
};
