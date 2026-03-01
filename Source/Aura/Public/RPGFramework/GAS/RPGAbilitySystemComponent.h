// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAssetTagsGet, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityGiven, URPGAbilitySystemComponent*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOutOfHealthSignature,AActor* /*Instigator*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilityStatusChangedSignature, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/)
DECLARE_DELEGATE_OneParam(FAbilitySpecAction, const FGameplayAbilitySpec&);
/**
 * 
 */
UCLASS()
class AURA_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityActorInfoSet();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void ApplyActionToAbilities(const FAbilitySpecAction& Action);
	
	
	void UpdateAbilityStatuses(int32 Level);
	void UnlockOrUpgradeAbility(const FGameplayTag& AbilityTag);
	
	
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	FOnEffectAssetTagsGet OnEffectAssetTagsGet;
	FOnAbilityGiven OnAbilityGiven;
	FOnOutOfHealthSignature OnOutOfHealth;
	FOnAbilityStatusChangedSignature OnAbilityStatusChanged;
	
	bool bStartupAbilitiesGiven = false;
	
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
	
	virtual void OnRep_ActivateAbilities() override;
};
