// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * 
 */

struct FRPGGameplayTags
{
public:
	static const FRPGGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attribute_Combat_Armor;
protected:
	
private:
	static FRPGGameplayTags GameplayTags;
};
