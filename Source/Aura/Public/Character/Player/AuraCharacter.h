// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Framework/CoreCharacterBase.h"
#include "AuraCharacter.generated.h"


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
	
private:
	virtual void InitAbilityActorInfo() override;
	
};
