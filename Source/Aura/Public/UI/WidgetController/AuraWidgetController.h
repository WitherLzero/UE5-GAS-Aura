// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "WidgetControllerBase.h"
#include "AuraWidgetController.generated.h"

class ACorePlayerController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
