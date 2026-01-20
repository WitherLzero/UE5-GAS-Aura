// Copyright rynnli


#include "RPGGameplayTags.h"
#include "GameplayTagsManager.h"

FRPGGameplayTags FRPGGameplayTags::GameplayTags;

void FRPGGameplayTags::InitializeNativeGameplayTags()
{

	GameplayTags.Attributes_Vital_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second")
		);

	GameplayTags.Attributes_Vital_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
		);

	GameplayTags.Attributes_Vital_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxHealth"),
		FString("Maximum amount of Health obtainable")
		);

	GameplayTags.Attributes_Vital_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxMana"),
		FString("Maximum amount of Mana obtainable")
		);

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage")
		);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage")
		);

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
		);

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
		);

	
	GameplayTags.Attributes_Combat_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.Armor"),
		FString("Reduces damage taken, improves Block Chance")
		);

	GameplayTags.Attributes_Combat_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance")
		);

	GameplayTags.Attributes_Combat_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.BlockChance"),
		FString("Chance to cut incoming damage in half")
		);

	GameplayTags.Attributes_Combat_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
		);

	GameplayTags.Attributes_Combat_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
		);

	GameplayTags.Attributes_Combat_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Combat.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
		);


	GameplayTags.Inputs_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.LMB"),
		FString("Input Tag for Left Mouse Button")
		);
	
	GameplayTags.Inputs_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.RMB"),
		FString("Input Tag for Right Mouse Button")
		);
	
	GameplayTags.Inputs_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.1"),
		FString("Input Tag for Key 1")
		);
	
	GameplayTags.Inputs_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.2"),
		FString("Input Tag for Key 2")
		);
	
	GameplayTags.Inputs_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.3"),
		FString("Input Tag for Key 3")
		);
	
	GameplayTags.Inputs_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.4"),
		FString("Input Tag for Key 4")
		);
	
}
