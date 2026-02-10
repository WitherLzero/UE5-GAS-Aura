// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;	
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	void RegisterWeaponMesh(USceneComponent* InMesh);
	
	UFUNCTION(BlueprintCallable)
	FVector GetCombatSocketLocation() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCombatTarget(AActor* InTarget) { CombatTarget = InTarget; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetCombatTarget() const { return CombatTarget; }	

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> WeaponMesh;
	
	UPROPERTY(EditAnywhere,Category= "Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	UPROPERTY(VisibleAnywhere,Category= "Combat")
	AActor* CombatTarget;
	
	UPROPERTY(EditAnywhere, Category= "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category= "Combat")
	FName FacingTargetName;
};
