// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/Player/CorePlayerState.h"
#include "AuraPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public ACorePlayerState
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
