// Copyright rynnli


#include "AuraGame/GAS/Abilities/AuraSummon.h"

#include "GameplayMechanics/Core/RPGAbilitySystemLibrary.h"

TArray<FVector> UAuraSummon::GetSummonLocations()
{
	TArray<FVector> SummonLocations;
	
	const FVector ActorLoc = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const TArray<FVector> Directions = URPGAbilitySystemLibrary::EvenlySpacedVectors(Forward,FVector::UpVector,SpawnSpread,NumMinions);
	
	for (const FVector& Vec : Directions)
	{
		FVector SpawnLocation = ActorLoc + Vec * FMath::FRandRange(MinSpawnDist,MaxSpawnDist);
		
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult,
			SpawnLocation + 300.f * FVector::UpVector,SpawnLocation + 300.f * FVector::DownVector,ECollisionChannel::ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			SpawnLocation = HitResult.ImpactPoint;
		}
		
		SummonLocations.Add(SpawnLocation);
	}
	
	return SummonLocations;
}

TSubclassOf<APawn> UAuraSummon::PickRandomMinionClass()
{
	if (MinionClasses.IsEmpty()) return nullptr;
	
	const int32 Selection = FMath::RandRange(0,MinionClasses.Num()-1);
	return MinionClasses[Selection];
}
