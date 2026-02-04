// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGGameplayAbilityBase.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageGameplayAbility : public URPGGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageType;
};
