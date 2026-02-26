// Copyright rynnli


#include "AuraGame/UI/WidgetController/AttributeMenuWidgetController.h"

#include "Aura/Public/RPGFramework/GAS/AttributeSets/RPGAttributeSetBase.h"
#include "AuraGame/Character/Player/AuraPlayerState.h"
#include "AuraGame/GAS/AttributeSets/CombatAttributeSet.h"
#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"
#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"
#include "RPGFramework/GAS/Data/AttributeInfo.h"

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

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChanged.AddLambda(
		[this](int32 NewPoints)
		{
			AttributePointsChanged.Broadcast(NewPoints);
		});
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const URPGAttributeSetBase* AS) const
{
	if (!ensure(AS)) return;

	for (auto& Pair : AS->TagsToAttributes)
	{
		FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
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
				FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				OnAttributeInfoChanged.Broadcast(Info);
			}
		);
	}
}
