// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "WidgetControllerBase.h"
#include "AuraWidgetController.generated.h"

class ACorePlayerController;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams(){}
	FWidgetControllerParams(UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerController* PC, APlayerState* PS)
		:AbilitySystemComponent(ASC),AttributeSet(AS),PlayerController(PC),PlayerState(PS){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
};

UCLASS()
class AURA_API UAuraWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
protected:
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
