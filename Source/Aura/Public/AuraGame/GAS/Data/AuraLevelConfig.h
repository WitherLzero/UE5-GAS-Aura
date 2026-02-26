// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGFramework/GAS/Data/LevelConfig.h"
#include "AuraLevelConfig.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraLevelConfig : public ULevelConfig
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	virtual int32 FindLevelForXP(int32 XP) const override;
	
	virtual int32 GetXPRequirement(int32 Level) const override;
	
	virtual int32 GetMaxLevel()const override;
	
	int32 GetAttributePointsReward(int32 Level) const;
	int32 GetSpellPointsReward(int32 Level) const;
};
