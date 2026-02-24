// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/Player/RPGPlayerState.h"
#include "AuraPlayerState.generated.h"

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
	
protected:
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAS;
	UPROPERTY()
	TObjectPtr<UCombatAttributeSet> CombatAS;
	
};
