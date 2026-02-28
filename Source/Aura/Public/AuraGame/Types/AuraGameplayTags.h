#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAuraGameplayTags 
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	static FAuraGameplayTags GameplayTags;
	
	
	/*
	 *  Attribute Tags
	 */
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
	 *  Damage Tags
	 */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	
	/*
	 *  Ability Tags
	 */
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	
	/*
	 *  Cooldown Tags
	 */
	FGameplayTag Cooldown_Fire_FireBolt;
	
	
};
