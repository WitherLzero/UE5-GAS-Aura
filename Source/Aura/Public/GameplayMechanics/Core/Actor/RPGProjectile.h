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

	// ================= Homing Mechanism =================
	
	/** Sets a living Actor as the homing target (Highest Priority). */
	UFUNCTION(BlueprintCallable, Category = "RPG Projectile|Homing")
	void SetHomingTargetActor(AActor* TargetActor);

	/** Sets a specific world location as the homing target. */
	UFUNCTION(BlueprintCallable, Category = "RPG Projectile|Homing")
	void SetHomingTargetLocation(const FVector& TargetLocation);

	/** Activates homing behavior and overrides basic projectile movement rules. */
	UFUNCTION(BlueprintCallable, Category = "RPG Projectile|Homing")
	void EnableHoming(float AccelerationMin = 1600.f, float AccelerationMax = 3200.f);

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHomingTrackerTick();
	
	void HandleOnHit();
	
	
	UPROPERTY(BlueprintReadOnly, Category = "RPG Projectile|Homing")
	TObjectPtr<AActor> HomingTargetActor;

	UPROPERTY(BlueprintReadOnly, Category = "RPG Projectile|Homing")
	FVector HomingTargetLocation;

	UPROPERTY(BlueprintReadOnly, Category = "RPG Projectile|Homing")
	bool bIsHomingToLocation = false;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
	FTimerHandle HomingTrackerTimer;
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
