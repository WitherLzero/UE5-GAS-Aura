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
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, bool bApplyDebuff = true) const;
	
	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Ability | Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Ability |Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Ability |Debuff")
	TSubclassOf<UGameplayEffect> DebuffCarrier;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability |Debuff")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability |Debuff")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability |Debuff")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability |Debuff")
	float DebuffDuration = 5.f;
};
