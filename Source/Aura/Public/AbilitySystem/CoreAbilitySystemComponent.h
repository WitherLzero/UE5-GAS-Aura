// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CoreAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAssetTagsGet, const FGameplayTagContainer& /*AssetTags*/)
/**
 * 
 */
UCLASS()
class AURA_API UCoreAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityActorInfoSet();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	
	FOnEffectAssetTagsGet OnEffectAssetTagsGet;
	
protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
