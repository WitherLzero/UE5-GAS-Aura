// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "RPGFramework/GAS/RPGAbilityTypes.h"
#include "RPGProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API ARPGProjectile : public AActor
{
	GENERATED_BODY()

public:
	ARPGProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void HandleOnHit();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category= "FXs | Niagara")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere, Category= "FXs | Sound")
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere, Category= "FXs | Sound")
	TObjectPtr<USoundBase> LoopingSound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingAudioComponent;
	
	bool bHit;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 10.f;
	
};
