// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * 
 */

struct FRPGGameplayTags
{
public:
	static const FRPGGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	/*
	 *  Attribute Tags
	 */
	FGameplayTag Attributes_Vital_HealthRegeneration;
	FGameplayTag Attributes_Vital_ManaRegeneration;
	FGameplayTag Attributes_Vital_MaxHealth;
	FGameplayTag Attributes_Vital_MaxMana;
	
	FGameplayTag Attributes_Meta_IncomingXP;
	
	
	/*
	 *  Input Tags
	 */
	FGameplayTag Inputs_LMB;
	FGameplayTag Inputs_RMB;
	FGameplayTag Inputs_1;
	FGameplayTag Inputs_2;
	FGameplayTag Inputs_3;
	FGameplayTag Inputs_4;
	FGameplayTag Inputs_Move;
	
	
	/*
	 *  Effect Tags
	 */
	FGameplayTag Effects_HitReact;
	
	
	/*
	*  Montage
	*/
	FGameplayTag Montage_Attack_Weapon;
	FGameplayTag Montage_Attack_LeftHand;
	FGameplayTag Montage_Attack_RightHand;
	FGameplayTag Montage_Attack_Tail;
	

	
private:
	static FRPGGameplayTags GameplayTags;
};
