// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Framework/BaseCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class AURA_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
