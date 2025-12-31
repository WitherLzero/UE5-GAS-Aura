// Copyright rynnli


#include "Framework/CorePlayerState.h"

#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "AbilitySystem/CoreAttributeSet.h"

ACorePlayerState::ACorePlayerState()
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UCoreAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UCoreAttributeSet>("AttributeSet");
}
