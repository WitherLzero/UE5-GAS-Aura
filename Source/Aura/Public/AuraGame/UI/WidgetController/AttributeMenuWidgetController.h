// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "../../../RPGFramework/UI/WidgetController/WidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FRPGAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoChanged, const FRPGAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeInfoChanged OnAttributeInfoChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerInfoChangedSignature AttributePointsChanged;
	
protected:
	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const class URPGAttributeSetBase* AS) const;
	void BindAttributeChangeDelegates(const class URPGAttributeSetBase* AS);
};
