// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"


class URPGAbilitySystemComponent;
class UUserWidgetBase;
struct FOnAttributeChangeData;

USTRUCT()
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UUserWidgetBase> MessageWidget;

	/*
	 *  To be added 
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowGet, FUIWidgetRow, Row);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChanged OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChanged OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChanged OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Messages")
	FOnMessageWidgetRowGet OnMessageWidgetRowGet;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | XP")
	FOnPlayerInfoChangedSignature OnXPPercentChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS | Level")
	FOnPlayerInfoChangedSignature OnPlayerLevelChanged;
protected:
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
	void HandleXPChanged(int32 NewXP);
	
	void HandleAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& NewSlot, const FGameplayTag& PreviousSlot);
	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	

};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
