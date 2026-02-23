// Copyright rynnli


#include "AuraGame/GAS/Data/AuraLevelConfig.h"

int32 UAuraLevelConfig::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 2 Information
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}

int32 UAuraLevelConfig::GetXPRequirement(int32 Level) const
{
	if (LevelUpInformation.IsValidIndex(Level))
	{
		return LevelUpInformation[Level].LevelUpRequirement;
	}
	return 0;
}

int32 UAuraLevelConfig::GetMaxLevel() const
{
	return LevelUpInformation.Num() - 1;
}
