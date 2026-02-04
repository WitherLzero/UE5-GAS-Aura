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
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UVitalAttributeSet> VitalAS;


	
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	
	UFUNCTION()
	void OnRep_Level(const int32& OldLevel);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UVitalAttributeSet* GetVitalAttributeSet() const { return VitalAS; }	
	FORCEINLINE int32 GetPlayerLevel()const { return Level; }
};
