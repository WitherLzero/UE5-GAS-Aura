#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
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
	// Combat set usually doesn't strictly need PreAttributeChange unless you have caps (like BlockChance <= 100%)
	// virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

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
};
