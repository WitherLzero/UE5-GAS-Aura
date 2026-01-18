// Copyright rynnli


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "RPGGameplayTags.h"
#include "AbilitySystem/AttributeSets/PrimaryAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UPrimaryAttributeSet* PrimaryAS = Cast<UPrimaryAttributeSet>(AbilitySystemComponent->GetAttributeSet(UPrimaryAttributeSet::StaticClass()));
	
	check(AttributeInfo);
	
	FRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(FRPGGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = PrimaryAS->GetStrength();
	OnAttributeInfoChanged.Broadcast(Info);
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	 
}
