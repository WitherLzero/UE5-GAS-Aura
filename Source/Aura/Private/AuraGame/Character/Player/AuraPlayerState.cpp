// Copyright rynnli


#include "AuraGame/Character/Player/AuraPlayerState.h"

#include "AuraGame/GAS/AttributeSets/CombatAttributeSet.h"
#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	PrimaryAS = CreateDefaultSubobject<UPrimaryAttributeSet>("PrimaryAttributeSet");
	CombatAS = CreateDefaultSubobject<UCombatAttributeSet>("CombatAttributeSet");
}
