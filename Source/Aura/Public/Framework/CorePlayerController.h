// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACorePlayerController();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
private:

	
	
	// Input Callbacks
	// TODO: Extend to Tag-distribution pattern 
	void Move(const FInputActionValue& InputActionValue);
	
	// helpers
	void UpdateMouse();
	
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputMappingContext> CurrentMappingContext;
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> MoveAction;
};
