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
	
protected:
	
private:
	static FRPGGameplayTags GameplayTags;
};
