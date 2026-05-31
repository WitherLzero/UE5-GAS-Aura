// Copyright rynnli


#include "AuraGame/GAS/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(const FGameplayTag& CharacterClassTag)
{
	return CharacterClassInfo.FindChecked(CharacterClassTag);
}
