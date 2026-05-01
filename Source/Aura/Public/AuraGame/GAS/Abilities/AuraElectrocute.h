// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayMechanics/Core/Abilities/RPGBeamSpell.h"
#include "AuraElectrocute.generated.h"

/**
 * Electrocute - Beam spell dealing lightning damage with shock propagation
 */
UCLASS()
class AURA_API UAuraElectrocute : public URPGBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
