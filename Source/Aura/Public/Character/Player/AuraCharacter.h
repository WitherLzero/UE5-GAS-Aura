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

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Input handlers
	virtual void Move(const FVector2D& InputAxis) override;
	
};
