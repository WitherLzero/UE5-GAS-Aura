// Copyright rynnli


#include "AbilitySystem/CoreAbilitySystemComponent.h"

void UCoreAbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UCoreAbilitySystemComponent::EffectApplied);
}


void UCoreAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	
}
