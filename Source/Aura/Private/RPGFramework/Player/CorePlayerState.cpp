// Copyright rynnli


#include "RPGFramework/Player/CorePlayerState.h"
#include "Net/UnrealNetwork.h"

#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"

ACorePlayerState::ACorePlayerState()
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	VitalAS = CreateDefaultSubobject<UVitalAttributeSet>("VitalAttributeSet");

}

void ACorePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACorePlayerState,Level);
}

void ACorePlayerState::OnRep_Level(const int32& OldLevel)
{
}
