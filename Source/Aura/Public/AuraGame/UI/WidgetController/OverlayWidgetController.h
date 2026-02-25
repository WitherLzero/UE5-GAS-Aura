// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "../../../RPGFramework/UI/WidgetController/WidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FRPGAbilityInfo;
class URPGAbilitySystemComponent;
class UAbilityInfo;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowGet, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityInfoGet, const FRPGAbilityInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UWidgetController
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

	UPROPERTY(BlueprintAssignable, Category="GAS | Messages")
	FOnAbilityInfoGet OnAbilityInfoGet;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | XP")
	FOnPlayerInfoChanged OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS | Level")
	FOnPlayerInfoChanged OnPlayerLevelChangedDelegate;
protected:
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
	void OnInitializeStartupAbilities(URPGAbilitySystemComponent* ASC);
	void OnXPChanged(int32 NewXP);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
