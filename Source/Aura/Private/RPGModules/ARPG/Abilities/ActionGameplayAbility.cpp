// Copyright rynnli


#include "RPGModules/ARPG/Abilities/ActionGameplayAbility.h"

#include "RPGModules/ARPG/Components/ActionComponent.h"
#include "RPGModules/Components/CombatComponent.h"

void UActionGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	CombatComponent = ActorInfo->AvatarActor->FindComponentByClass<UCombatComponent>();
	ActionComponent = ActorInfo->AvatarActor->FindComponentByClass<UActionComponent>();
}
