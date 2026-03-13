// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayMechanics/Core/Interaction/CombatInterface.h"
#include "RPGFramework/Character/CoreCharacterBase.h"
#include "AuraCharacterBase.generated.h"

class UVitalityComponent;
class UActionComponent;
class UCombatComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACoreCharacterBase, public  ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	/* Combat Interface*/
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
	virtual UNiagaraSystem* GetHitReactEffect_Implementation() const override;
	
	virtual void SetCombatTarget_Implementation(AActor* InTarget) override;
	/* end Combat Interface*/
	
	UFUNCTION()
	virtual void HandleDeath(AActor* DeadActor, AActor* KillerActor);
	
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UVitalityComponent> VitalityComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCombatComponent> CombatComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UActionComponent> ActionComp;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	UPROPERTY(BlueprintReadOnly, Category = "Death")
	float LifeSpan = 3.f;

};
