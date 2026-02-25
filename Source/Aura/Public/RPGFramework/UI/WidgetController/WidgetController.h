// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.generated.h"

class UAbilitySystemComponent;
class ARPGPlayerController;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams(){}
	FWidgetControllerParams(UAbilitySystemComponent* ASC, APlayerController* PC, APlayerState* PS)
		:AbilitySystemComponent(ASC),PlayerController(PC),PlayerState(PS){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
};

UCLASS()
class AURA_API UWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
