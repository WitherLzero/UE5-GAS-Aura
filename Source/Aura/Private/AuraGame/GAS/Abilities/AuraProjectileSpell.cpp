// Copyright rynnli


#include "AuraGame/GAS/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGame/Actor/AuraProjectile.h"
#include "RPGModules/Components/CombatComponent.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLoc)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (UCombatComponent* CombatComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatComponent>())
	{
		const FVector SocketLocation = CombatComp->GetCombatSocketLocation();
		const FRotator Rotation = (ProjectileTargetLoc - SocketLocation).Rotation();
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		
		Projectile->DamageEffectSpecHandle = MakeDamageEffectSpecHandle(GetAvatarActorFromActorInfo(),Projectile);
		
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	
}
