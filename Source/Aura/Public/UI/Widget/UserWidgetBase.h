// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UUserWidgetBase : public UObject
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
public:
	UFUNCTION(blueprintCallable)
	virtual void SetWidgetController(UObject* InWidgetController);
};
