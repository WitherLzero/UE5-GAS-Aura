// Copyright rynnli


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "AbilitySystem/CoreAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCoreAttributeSet* PlayerAttributeSet = CastChecked<UCoreAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(PlayerAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PlayerAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PlayerAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PlayerAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UCoreAttributeSet* PlayerAttributeSet = CastChecked<UCoreAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnMaxHealthChanged.Broadcast(Data.NewValue);});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnManaChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnMaxManaChanged.Broadcast(Data.NewValue);});

	Cast<UCoreAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTagsGet.AddLambda(
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


