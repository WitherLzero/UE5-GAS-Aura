// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/Game/RPGGameModeBase.h"
#include "AuraGameMode.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameMode : public ARPGGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
