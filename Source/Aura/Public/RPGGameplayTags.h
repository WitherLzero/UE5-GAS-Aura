// Copyright rynnli

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FRPGGameplayTags
{
public:
	static const FRPGGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
protected:
	
private:
	static FRPGGameplayTags GameplayTags;
};
