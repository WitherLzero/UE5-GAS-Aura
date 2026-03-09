// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayMechanics/Core/Abilities/RPGProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public URPGProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;
};
