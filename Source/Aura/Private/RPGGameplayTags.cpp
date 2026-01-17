// Copyright rynnli


#include "RPGGameplayTags.h"
#include "GameplayTagsManager.h"

FRPGGameplayTags FRPGGameplayTags::GameplayTags;

void FRPGGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attribute_Combat_Armor =  UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.Armor"), FString("Reduces damage taken, improves Block Chance"));
}
