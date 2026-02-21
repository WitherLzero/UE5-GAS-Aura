// Copyright rynnli


#include "AuraGame/UI/WidgetController/OverlayWidgetController.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"

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

	if (URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASC);
		}
		else
		{
			ASC->OnAbilityGiven.AddUObject(this,&ThisClass::OnInitializeStartupAbilities);
		}
		
		
		ASC->OnEffectAssetTagsGet.AddLambda(
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
	

}

void UOverlayWidgetController::OnInitializeStartupAbilities(URPGAbilitySystemComponent* ASC)
{
}


