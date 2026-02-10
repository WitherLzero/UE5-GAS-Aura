// Copyright rynnli


#include "CombatConfig.h"


FName UCombatConfig::GetSocketNameByTag(const FGameplayTag& Tag) const
{
	return TagToSocketMap.FindChecked(Tag);
}
