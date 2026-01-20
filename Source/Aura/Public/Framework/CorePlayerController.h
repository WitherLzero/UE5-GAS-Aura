// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

enum class ERPGInputEvent : uint8;
class URPGInputConfig;
class IEnemyInterface;
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
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
private:
	// Input Callbacks
	// TODO: Extend to Tag-distribution pattern 
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	
	// helpers
	void UpdateMouse();
	 
	// IA Callbacks
	void OnInputTagPressed(FGameplayTag InputTag);
	void OnInputTagReleased(FGameplayTag InputTag);
	void OnInputTagHeld(const FInputActionValue& InputActionValue,FGameplayTag InputTag);
	
	// Input handler 
	void ProcessInputTag(FGameplayTag InputTag, ERPGInputEvent EventType, const FInputActionValue& InputActionValue = FInputActionValue());
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputMappingContext> CurrentMappingContext;
	
	UPROPERTY(EditDefaultsOnly,Category= "Input")
	TObjectPtr<URPGInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
};
