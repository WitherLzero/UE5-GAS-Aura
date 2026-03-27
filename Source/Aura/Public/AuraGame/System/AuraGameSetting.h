// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AuraGame/GAS/Data/CharacterClassInfo.h"
#include "Engine/DeveloperSettings.h"
#include "AuraGameSetting.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="Aura Game Settings"))
class AURA_API UAuraGameSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Aura Data Assets")
	TSoftObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
