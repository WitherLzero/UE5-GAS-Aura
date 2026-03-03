// Copyright rynnli


#include "AuraGame/UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGame/Character/Player/AuraPlayerState.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/Data/AbilityInfo.h"



void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnInitializeStartupAbilities(Cast<URPGAbilitySystemComponent>(AbilitySystemComponent));
	
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 NewLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(CurrentSpellPoints, StatusTag, bEnableSpendPoints, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
			}
			
			if (AbilityInfo)
			{
				FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				OnAbilityInfoGet.Broadcast(Info);
			}
		});
	
	GetAuraPS()->OnSpellPointsChanged.AddLambda(
		[this](int32 NewPoints)
		{
			SpellPointsChanged.Broadcast(NewPoints);
			CurrentSpellPoints = NewPoints;
			
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(CurrentSpellPoints, SelectedAbility.Status, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}else
	{
		AbilityStatus = GetASC()->GetStatusFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(SpellPoints, AbilityStatus, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);	
}

void USpellMenuWidgetController::SpellGlobeDeselected()
{
	SelectedAbility.Ability = FRPGGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FRPGGameplayTags::Get().Abilities_Status_Locked;
	
	OnSpellGlobeSelected.Broadcast(false, false, FString(), FString());	
	
}

void USpellMenuWidgetController::SpendSpellPoints()
{
	if (GetAuraPS())
	{
		GetAuraPS()->SpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::ShouldEnableButtons(const int32 SpellPoints, const FGameplayTag& AbilityStatus,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
