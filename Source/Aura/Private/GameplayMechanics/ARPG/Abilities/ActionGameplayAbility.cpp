// Copyright rynnli


#include "GameplayMechanics/ARPG/Abilities/ActionGameplayAbility.h"

#include "GameplayMechanics/ARPG/Components/ActionComponent.h"

void UActionGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	ActionComponent = ActorInfo->AvatarActor->FindComponentByClass<UActionComponent>();
}
