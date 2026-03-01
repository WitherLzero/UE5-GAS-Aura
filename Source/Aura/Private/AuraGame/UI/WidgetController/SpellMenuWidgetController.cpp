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
				OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip);
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
			OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip);
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
	const bool bTagValid = AbilityTag.IsValid();
	const FGameplayAbilitySpec* AbilitySpec = GetASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || !bSpecValid)
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
	OnSpellGlobeSelected.Broadcast(bEnableSpendPoints,bEnableEquip);
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
