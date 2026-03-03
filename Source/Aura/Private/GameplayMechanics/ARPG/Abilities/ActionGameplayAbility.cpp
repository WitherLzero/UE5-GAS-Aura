// Copyright rynnli


#include "GameplayMechanics/ARPG/Abilities/ActionGameplayAbility.h"

#include "GameplayMechanics/ARPG/Components/ActionComponent.h"
#include "GameplayMechanics/Core/Components/CombatComponent.h"

void UActionGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	CombatComponent = ActorInfo->AvatarActor->FindComponentByClass<UCombatComponent>();
	ActionComponent = ActorInfo->AvatarActor->FindComponentByClass<UActionComponent>();
}
