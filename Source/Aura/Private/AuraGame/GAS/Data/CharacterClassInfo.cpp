// Copyright rynnli


#include "AuraGame/GAS/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}
