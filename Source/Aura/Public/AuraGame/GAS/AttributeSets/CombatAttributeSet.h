#pragma once

#include "CoreMinimal.h"
#include "Aura/Public/RPGFramework/GAS/AttributeSets/RPGAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "CombatAttributeSet.generated.h"

/**
 * Combat Attributes: Armor, Penetration, Block, Crit, etc.
 */
UCLASS()
class AURA_API UCombatAttributeSet : public URPGAttributeSetBase
{
	GENERATED_BODY()
public:
	UCombatAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	/*
	 * Secondary Attributes (Combat)
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category = "Combat Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArmorPenetration, Category = "Combat Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, ArmorPenetration)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockChance, Category = "Combat Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, BlockChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category = "Combat Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CriticalHitChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category = "Combat Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CriticalHitDamage)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitResistance, Category = "Combat Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CriticalHitResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category = "Combat Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LightningResistance, Category = "Combat Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, LightningResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArcaneResistance, Category = "Combat Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, ArcaneResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category = "Combat Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, PhysicalResistance)

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
};
