// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/UI/WidgetController/WidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Spells")
	FOnPlayerInfoChangedSignature SpellPointsChanged;
};
