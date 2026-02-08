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



