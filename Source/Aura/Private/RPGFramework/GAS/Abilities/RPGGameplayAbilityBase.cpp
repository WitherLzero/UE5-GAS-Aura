// Copyright rynnli


#include "RPGFramework/GAS/Abilities/RPGGameplayAbilityBase.h"

FString URPGGameplayAbilityBase::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum ", Level);
}

FString URPGGameplayAbilityBase::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString URPGGameplayAbilityBase::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}
