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
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(AActor* SourceActor, UObject* SourceObject) const;
	
	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
	
};
