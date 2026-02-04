// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "../../../RPGFramework/GAS/Abilities/DamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLoc);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
