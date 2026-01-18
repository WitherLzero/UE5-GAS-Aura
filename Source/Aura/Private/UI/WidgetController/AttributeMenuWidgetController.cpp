// Copyright rynnli


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSets/PrimaryAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UPrimaryAttributeSet* PrimaryAS = Cast<UPrimaryAttributeSet>(AbilitySystemComponent->GetAttributeSet(UPrimaryAttributeSet::StaticClass()));
	
	check(AttributeInfo);
	
	for (auto& Pair : PrimaryAS->TagsToAttributes)
	{
		FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(PrimaryAS);
		OnAttributeInfoChanged.Broadcast(Info);
	}
	
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	 
}
