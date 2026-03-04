// Copyright rynnli


#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"

#include "GameplayMechanics/Core/RPGAbilitySystemLibrary.h"
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

// TODO: 考虑如何把业务逻辑下放 
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
				ClientUpdateAbilityStatus(Info.AbilityTag,FRPGGameplayTags::Get().Abilities_Status_Eligible,AbilitySpec.Level);
			}
			
		}
	}
}

void URPGAbilitySystemComponent::UnlockOrUpgradeAbility(const FGameplayTag& AbilityTag)
{
	if (!GetOwner()->HasAuthority()) return; 
	
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag,Status,AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

bool URPGAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (URPGGameplayAbilityBase* Ability = Cast<URPGGameplayAbilityBase>(AbilitySpec->Ability))
		{
			OutDescription = Ability->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = Ability->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = URPGAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FRPGGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = URPGGameplayAbilityBase::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

void URPGAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag& PrevInput = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);
		
		const FRPGGameplayTags GameplayTags	= FRPGGameplayTags::Get();
		const bool bCanEquip = (Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked);
		if (bCanEquip)
		{
			// Remove this InputTag from any ability that has it
			// Clear and unbind current ability's input
			UnbindAbilitiesFromInput(InputTag);
			ClearAbilityInput(AbilitySpec);
			
			AbilitySpec->DynamicAbilityTags.AddTag(InputTag);
			if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			ClientEquipAbility(AbilityTag,Status,InputTag,PrevInput);
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

void URPGAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
																	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	OnEffectAssetTagsGet.Broadcast(TagContainer);
	
}

void URPGAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	OnAbilityStatusChanged.Broadcast(AbilityTag,StatusTag,AbilityLevel);
}

void URPGAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const FGameplayTag& NewInputTag, const FGameplayTag& PreviousInputTag)
{
	OnAbilityEquipped.Broadcast(AbilityTag,StatusTag,NewInputTag,PreviousInputTag);
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

void URPGAbilitySystemComponent::ClearAbilityInput(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag& InputTag = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(InputTag);
	MarkAbilitySpecDirty(*Spec);
}

void URPGAbilitySystemComponent::UnbindAbilitiesFromInput(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& Spec: GetActivatableAbilities())
	{
		if (AbilityHasInputTag(&Spec,InputTag))
		{
			ClearAbilityInput(&Spec);
		}
	}
}

bool URPGAbilitySystemComponent::AbilityHasInputTag(FGameplayAbilitySpec* Spec, const FGameplayTag& InputTag)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(InputTag))
		{
			return true;
		}
	}
	return false;
}


