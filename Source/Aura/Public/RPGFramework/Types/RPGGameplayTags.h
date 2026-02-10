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

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Combat_Armor;
	FGameplayTag Attributes_Combat_ArmorPenetration;
	FGameplayTag Attributes_Combat_BlockChance;
	FGameplayTag Attributes_Combat_CriticalHitChance;
	FGameplayTag Attributes_Combat_CriticalHitDamage;
	FGameplayTag Attributes_Combat_CriticalHitResistance;
	
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
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
	 *  Damage Tags
	 */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	
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
	

	
private:
	static FRPGGameplayTags GameplayTags;
};
