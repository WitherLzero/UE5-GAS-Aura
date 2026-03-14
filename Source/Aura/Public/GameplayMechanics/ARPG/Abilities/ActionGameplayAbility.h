// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/GAS/Abilities/DamageGameplayAbility.h"
#include "ActionGameplayAbility.generated.h"

class UActionComponent;
class UCombatComponent;
/**
 * 
 */
UCLASS()
class AURA_API UActionGameplayAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActionComponent> ActionComponent;

};
