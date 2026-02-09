// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AuraGame/Character/AuraCharacterBase.h"
#include "AuraGame/GAS/Data/CharacterClassInfo.h"
#include "RPGFramework/Interaction/EnemyInterface.h"
#include "AuraGame/UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

class UActionComponent;
class UBehaviorTree;
class ARPGAIController;
class UCombatComponent;
class UWidgetComponent;
class UCombatAttributeSet;
class UPrimaryAttributeSet;
class UVitalAttributeSet;

UCLASS()
class AURA_API AEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChanged OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitDefaultAttributes() const override;
	
	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
    /* end Enemy Interface */
	
	/* Combat Interface */
	virtual int32 GetCharacterLevel() const override;
	/* end Combat Interface */
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	
	UPROPERTY()
	TObjectPtr<UVitalAttributeSet> VitalAS;
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAS;
	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> CombatAS;
	
	UPROPERTY()
	TObjectPtr<ARPGAIController> AIController;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
	
	UPROPERTY(EditAnywhere,Category = "Character Class Defaults")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
private:

	

};
