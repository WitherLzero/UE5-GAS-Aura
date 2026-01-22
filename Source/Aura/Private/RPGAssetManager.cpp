// Copyright rynnli


#include "RPGAssetManager.h"

#include "AbilitySystemGlobals.h"

#include "RPGGameplayTags.h"

URPGAssetManager& URPGAssetManager::Get()
{
	check(GEngine);
	return  *Cast<URPGAssetManager>(GEngine->AssetManager);
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FRPGGameplayTags::InitializeNativeGameplayTags();
	
	UAbilitySystemGlobals::Get().InitGlobalData();
}
