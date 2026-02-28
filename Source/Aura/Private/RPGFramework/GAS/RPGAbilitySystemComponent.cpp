// Copyright rynnli


#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"

#include "RPGFramework/GAS/RPGAbilitySystemLibrary.h"
#include "RPGFramework/GAS/Abilities/RPGGameplayAbilityBase.h"
#include "RPGFramework/GAS/Data/AbilityInfo.h"
#include "RPGFramework/Types/RPGGameplayTags.h"

void URPGAbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&URPGAbilitySystemComponent::ClientEffectApplied);
}

void URPGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const URPGGameplayAbilityBase* Ability = Cast<URPGGameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FRPGGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	OnAbilityGiven.Broadcast(this);
}

void URPGAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void URPGAbilitySystemComponent::ApplyActionToAbilities(const FAbilitySpecAction& Action)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Action.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

void URPGAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = URPGAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (AbilityInfo)
	{
		for (const FRPGAbilityInfo& Info: AbilityInfo->AbilityInfos)
		{
			if (!Info.AbilityTag.IsValid()) continue;
			if (Level < Info.LevelRequirement) continue;
			if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability,1);
				AbilitySpec.DynamicAbilityTags.AddTag(FRPGGameplayTags::Get().Abilities_Status_Eligible);
				GiveAbility(AbilitySpec);
				
				MarkAbilitySpecDirty(AbilitySpec);
				ClientUpdateAbilityStatus(Info.AbilityTag,FRPGGameplayTags::Get().Abilities_Status_Eligible);
			}
			
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

FGameplayTag URPGAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag URPGAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Inputs"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag URPGAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();	
}

FGameplayAbilitySpec* URPGAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void URPGAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag)
{
	OnAbilityStatusChanged.Broadcast(AbilityTag,StatusTag);
}

void URPGAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		OnAbilityGiven.Broadcast(this);
	}
}


void URPGAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	OnEffectAssetTagsGet.Broadcast(TagContainer);
	
}
