// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	void RegisterWeaponMesh(USceneComponent* InMesh);
	FVector GetCombatSocketLocation() const;
	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category= "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category= "Combat")
	FName FacingTargetName;
};
