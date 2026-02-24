// Copyright rynnli


#include "RPGFramework/Types/RPGGameplayTags.h"
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
	
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
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
	
	GameplayTags.Inputs_Move = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Inputs.Move"),
		FString("Input Tag for Movement")
		);
	
	
	
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
FString("Effect Tag for HitReact")
	);
	
	/*
	 *  Montage
	 */
	
	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Weapon"),
		FString("Weapon")
		);
	
	GameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LeftHand"),
		FString("Left Hand")
		);
	
	GameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RightHand"),
		FString("Right Hand")
		);		
	
	GameplayTags.Montage_Attack_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Tail"),
		FString("Tail")
		);			
}
