// Copyright rynnli


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/RPGAttributeSetBase.h"
#include "AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "AbilitySystem/AttributeSets/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSets/VitalAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	
	const UPrimaryAttributeSet* PrimaryAS = Cast<UPrimaryAttributeSet>(AbilitySystemComponent->GetAttributeSet(UPrimaryAttributeSet::StaticClass()));
	const UCombatAttributeSet* CombatAS = Cast<UCombatAttributeSet>(AbilitySystemComponent->GetAttributeSet(UCombatAttributeSet::StaticClass()));
	const UVitalAttributeSet* VitalAS = Cast<UVitalAttributeSet>(AbilitySystemComponent->GetAttributeSet(UVitalAttributeSet::StaticClass()));
	
	BroadcastAttributeInfo(PrimaryAS);
	BroadcastAttributeInfo(CombatAS);
	BroadcastAttributeInfo(VitalAS);
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	
	const UPrimaryAttributeSet* PrimaryAS = Cast<UPrimaryAttributeSet>(AbilitySystemComponent->GetAttributeSet(UPrimaryAttributeSet::StaticClass()));
	const UCombatAttributeSet* CombatAS = Cast<UCombatAttributeSet>(AbilitySystemComponent->GetAttributeSet(UCombatAttributeSet::StaticClass()));
	const UVitalAttributeSet* VitalAS = Cast<UVitalAttributeSet>(AbilitySystemComponent->GetAttributeSet(UVitalAttributeSet::StaticClass()));
	
	BindAttributeChangeDelegates(PrimaryAS);
	BindAttributeChangeDelegates(CombatAS);
	BindAttributeChangeDelegates(VitalAS);

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const URPGAttributeSetBase* AS) const
{
	if (!ensure(AS)) return;

	for (auto& Pair : AS->TagsToAttributes)
	{
		FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		OnAttributeInfoChanged.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindAttributeChangeDelegates(const URPGAttributeSetBase* AS)
{
	if (!ensure(AS)) return;

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
			.AddLambda([this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				OnAttributeInfoChanged.Broadcast(Info);
			}
		);
	}
}
