// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPGGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API URPGGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	FGameplayTag StartupInputTag;
	
};
