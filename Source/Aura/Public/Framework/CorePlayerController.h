// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

class UDamageTextComponent;
class UCoreAbilitySystemComponent;
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

	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(ACharacter* TargetCharacter, float DamageAmount, bool bBlockedHit, bool bCriticalHit);
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
private:
	// helpers
	void UpdateMouse();
	void CursorTrace();
	 
	// IA Callbacks
	void OnInputTagPressed(FGameplayTag InputTag);
	void OnInputTagReleased(FGameplayTag InputTag);
	void OnInputTagHeld(const FInputActionValue& InputActionValue,FGameplayTag InputTag);
	
	// Input handler 
	void ProcessInputTag(FGameplayTag InputTag, ERPGInputEvent EventType, const FInputActionValue& InputActionValue = FInputActionValue());
	
	UCoreAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputMappingContext> CurrentMappingContext;
	
	UPROPERTY(EditDefaultsOnly,Category= "Input")
	TObjectPtr<URPGInputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextCompClass;
	
	UPROPERTY()
	TObjectPtr<UCoreAbilitySystemComponent> AbilitySystemComponent;
	
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	
public:
	bool GetCursorHit(FHitResult& HitResult);
	bool HitEnemyActor() const { return ThisActor?true:false;}
};
