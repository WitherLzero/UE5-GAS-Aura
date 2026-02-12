// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/Character/CoreCharacterBase.h"
#include "AuraCharacterBase.generated.h"

class UVitalityComponent;
class UActionComponent;
class UCombatComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACoreCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	UFUNCTION()
	virtual void OnDeathCallbacks(AActor* DeadActor);
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UVitalityComponent> VitalityComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCombatComponent> CombatComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UActionComponent> ActionComp;	
	
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
