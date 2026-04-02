// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/GAS/Abilities/RPGGameplayAbilityBase.h"
#include "RPGPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API URPGPassiveAbility : public URPGGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
