#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/GAS/AttributeSets/RPGAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "PrimaryAttributeSet.generated.h"

/**
 * Primary Attributes: Strength, Intelligence, Resilience, Vigor
 */
UCLASS()
class AURA_API UPrimaryAttributeSet : public URPGAttributeSetBase
{
	GENERATED_BODY()
public:
	UPrimaryAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*
	 * Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UPrimaryAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UPrimaryAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UPrimaryAttributeSet, Resilience)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UPrimaryAttributeSet, Vigor)

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
};
