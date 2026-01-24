// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Framework/CoreCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Enemy.generated.h"

class UCombatAttributeSet;
class UPrimaryAttributeSet;
class UVitalAttributeSet;

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
	
	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
    /* end Enemy Interface */
	
	/* Combat Interface */
	virtual int32 GetCharacterLevel() const override;
	/* end Combat Interface */
	
	UPROPERTY()
	TObjectPtr<UVitalAttributeSet> VitalAS;
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAS;
	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> CombatAS;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Level = 1;
	
	
private:

	

};
