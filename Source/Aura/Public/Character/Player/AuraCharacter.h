// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Framework/CoreCharacterBase.h"
#include "AuraCharacter.generated.h"


class ACorePlayerController;
class USplineComponent;

UCLASS()
class AURA_API AAuraCharacter : public ACoreCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
protected:
	virtual void BeginPlay() override;
	

	/* Combat Interface */
	virtual int32 GetCharacterLevel() const override;
	/* end Combat Interface */
	
	/* Input Interactable */
	virtual bool HandleNativeInput(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value) override;
	/* end Input Interactable*/
	
	// Input handlers
	virtual bool OnNativeInput_Implementation(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value) override;
	virtual void Move(const FVector2D& InputAxis) override;
	bool HoldToMove();
	bool SetupNavPoints();
	void AutoRun();
	
	
	UPROPERTY()
	TObjectPtr<ACorePlayerController> PlayerController;
	
private:
	virtual void InitAbilityActorInfo() override;
	ACorePlayerController* GetPC();
	
	/*
	 *  Click-to-Move vars
	 */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
