// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetControllerBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API UWidgetControllerBase : public UObject
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY( BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


};
