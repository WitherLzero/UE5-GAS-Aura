// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Framework/CoreCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Enemy.generated.h"

UCLASS()
class AURA_API AEnemy : public ACoreCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	
private:

	

};
