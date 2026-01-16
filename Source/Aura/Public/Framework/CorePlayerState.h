// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CorePlayerState.generated.h"

class UCombatAttributeSet;
class UPrimaryAttributeSet;
class UVitalAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACorePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ACorePlayerState();
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UVitalAttributeSet> VitalAS;
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAS;
	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> CombatAS;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UVitalAttributeSet* GetVitalAttributeSet() const { return VitalAS; }
	UPrimaryAttributeSet* GetPrimaryAttributeSet() const { return PrimaryAS; }
	UCombatAttributeSet* GetCombatAttributeSet() const { return CombatAS; }
};
