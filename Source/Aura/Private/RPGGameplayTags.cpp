// Copyright rynnli


#include "RPGGameplayTags.h"
#include "GameplayTagsManager.h"

void FRPGGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.Armor"), FString("Reduces damage taken, improves Block Chance"));
}
