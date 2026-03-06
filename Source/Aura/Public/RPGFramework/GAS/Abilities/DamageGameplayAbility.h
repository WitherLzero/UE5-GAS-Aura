// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGGameplayAbilityBase.h"
#include "RPGFramework/GAS/RPGAbilityTypes.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageGameplayAbility : public URPGGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	
	UFUNCTION(BlueprintPure, Category = "Damage")
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;
};
