// Copyright rynnli


#include "AuraGame/Character/Player/AuraPlayerState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGame/GAS/AttributeSets/CombatAttributeSet.h"
#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	PrimaryAS = CreateDefaultSubobject<UPrimaryAttributeSet>("PrimaryAttributeSet");
	CombatAS = CreateDefaultSubobject<UCombatAttributeSet>("CombatAttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AAuraPlayerState::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (AttributePoints > 0)
	{
		ServerUpgradeAttribute(AttributeTag);
	}
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

void AAuraPlayerState::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetPawn(), AttributeTag, Payload);
	AddToAttributePoints(-1);
}

void AAuraPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	 OnSpellPointsChanged.Broadcast(SpellPoints);
}
