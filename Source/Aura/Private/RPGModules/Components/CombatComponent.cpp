// Copyright rynnli


#include "RPGModules/Components/CombatComponent.h"
#include "MotionWarpingComponent.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UCombatComponent::RegisterWeaponMesh(USceneComponent* InMesh)
{
	WeaponMesh = InMesh;
}


TArray<FTaggedMontage> UCombatComponent::GetAttackMontages_Implementation()
{
	return AttackMontages;
}


UAnimMontage* UCombatComponent::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}


FVector UCombatComponent::GetCombatSocketLocation() const
{
	check(WeaponMesh);
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}




void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}



