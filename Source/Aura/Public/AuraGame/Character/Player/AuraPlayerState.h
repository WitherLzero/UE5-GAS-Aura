// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/Player/RPGPlayerState.h"
#include "AuraPlayerState.generated.h"

struct FGameplayTag;
class UCombatAttributeSet;
class UPrimaryAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public ARPGPlayerState
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	FOnPlayerStatChanged OnAttributePointsChanged;
	FOnPlayerStatChanged OnSpellPointsChanged;
protected:
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAS;
	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> CombatAS;
	
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 1;
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
public:
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void AddToAttributePoints(int32 InPoints);
	void AddToSpellPoints(int32 InPoints);
};
