// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "RPGFramework/GAS/Abilities/RPGGameplayAbilityBase.h"
#include "AuraSummon.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummon : public URPGGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSummonLocations();
	
protected:
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	int32 NumMinions = 5;
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MinSpawnDist = 100.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MaxSpawnDist = 400.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float SpawnSpread = 90.f;
};
