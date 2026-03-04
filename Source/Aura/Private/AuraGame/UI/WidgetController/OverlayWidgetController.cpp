// Copyright rynnli


#include "AuraGame/UI/WidgetController/OverlayWidgetController.h"

#include "AuraGame/Character/Player/AuraPlayerState.h"
#include "AuraGame/GAS/Data/AuraLevelConfig.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"
#include "RPGFramework/GAS/Data/AbilityInfo.h"
#include "RPGFramework/Types/RPGGameplayTags.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	const UVitalAttributeSet* VitalSet = Cast<UVitalAttributeSet>(AbilitySystemComponent->GetAttributeSet(UVitalAttributeSet::StaticClass()));
	if (VitalSet)
	{
		OnHealthChanged.Broadcast(VitalSet->GetHealth());
		OnMaxHealthChanged.Broadcast(VitalSet->GetMaxHealth());
		OnManaChanged.Broadcast(VitalSet->GetMana());
		OnMaxManaChanged.Broadcast(VitalSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UVitalAttributeSet* VitalSet = Cast<UVitalAttributeSet>(AbilitySystemComponent->GetAttributeSet(UVitalAttributeSet::StaticClass()));
	if (VitalSet)
	{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			VitalSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			VitalSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnMaxHealthChanged.Broadcast(Data.NewValue);});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			VitalSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnManaChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			VitalSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnMaxManaChanged.Broadcast(Data.NewValue);});
	}

	if (GetASC())
	{
		if (GetASC()->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(GetASC());
		}
		else
		{
			GetASC()->OnAbilityGiven.AddUObject(this,&ThisClass::OnInitializeStartupAbilities);
		}
		
		GetASC()->OnAbilityEquipped.AddUObject(this,&ThisClass::HandleAbilityEquipped);
		
		GetASC()->OnEffectAssetTagsGet.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for(const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag Message = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(Message))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
						OnMessageWidgetRowGet.Broadcast(*Row);
					}				
				}
			}	
		);		
	}
	
	if (GetAuraPS())
	{
		GetAuraPS()->OnXPChanged.AddUObject(this,&ThisClass::HandleXPChanged);
		GetAuraPS()->OnLevelChanged.AddLambda(
			[this](int32 NewLevel)
			{
				OnPlayerLevelChanged.Broadcast(static_cast<float>(NewLevel));
			});
	} 

}



void UOverlayWidgetController::HandleXPChanged(int32 NewXP)
{
	const ARPGPlayerState* PS = Cast<ARPGPlayerState>(PlayerState);
	const UAuraLevelConfig* LevelUpInfo = Cast<UAuraLevelConfig>(PS->LevelConfig);
	checkf(LevelUpInfo,TEXT("Unable to find LevelConfig. Please fill out in PlayerState: %s"),*GetNameSafe(PS));
	
	const int32 CurrentLevel = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->GetMaxLevel();
	
	if (CurrentLevel <= MaxLevel && CurrentLevel>0)
	{
		const int32 ThisLevelXPRequirement = LevelUpInfo->GetXPRequirement(CurrentLevel);
		const int32 LastLevelXPRequirement = LevelUpInfo->GetXPRequirement(CurrentLevel-1);
		
		const int32 DeltaXPRequirement = ThisLevelXPRequirement - LastLevelXPRequirement;
		const int32 XPForThisLevel = NewXP - LastLevelXPRequirement;
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaXPRequirement);
		OnXPPercentChanged.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::HandleAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& NewSlot, const FGameplayTag& PreviousSlot)
{
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
	
	FRPGAbilityInfo LastSlotInfo;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	OnAbilityInfoGet.Broadcast(LastSlotInfo);
	
	FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = NewSlot;
	OnAbilityInfoGet.Broadcast(Info);
}


