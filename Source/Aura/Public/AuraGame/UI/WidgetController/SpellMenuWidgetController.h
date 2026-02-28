// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Spells")
	FOnPlayerInfoChangedSignature SpellPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnSpellGlobeSelectedSignature OnSpellGlobeSelected;
	
private:
	void ShouldEnableButtons(const int32 SpellPoints,const FGameplayTag& AbilityStatus, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
};
