
#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "RPGFramework/Types/RPGGameplayTags.h"

UPrimaryAttributeSet::UPrimaryAttributeSet()
{
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
	
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength,GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience,GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor,GetVigorAttribute);
}

void UPrimaryAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPrimaryAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrimaryAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrimaryAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrimaryAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
}


void UPrimaryAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrimaryAttributeSet, Strength, OldStrength);
}

void UPrimaryAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrimaryAttributeSet, Intelligence, OldIntelligence);
}

void UPrimaryAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrimaryAttributeSet, Resilience, OldResilience);
}

void UPrimaryAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrimaryAttributeSet, Vigor, OldVigor);
}
