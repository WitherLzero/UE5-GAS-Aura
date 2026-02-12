// Copyright rynnli


#include "RPGModules/Data/CombatConfig.h"


FName UCombatConfig::GetSocketNameByTag(const FGameplayTag& Tag) const
{
	return TagToSocketMap.FindChecked(Tag);
}
