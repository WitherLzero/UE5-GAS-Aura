// Copyright rynnli


#include "AuraGame/GAS/Abilities/AuraSummon.h"

TArray<FVector> UAuraSummon::GetSummonLocations()
{
	TArray<FVector> SummonLocations;
	
	const FVector ActorLoc = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const float DeltaSpread = SpawnSpread / NumMinions;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis( -SpawnSpread/2.f, FVector::UpVector);
	for (int i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis( DeltaSpread*i, FVector::UpVector);
		FVector SpawnLocation = ActorLoc + Direction * FMath::FRandRange(MinSpawnDist,MaxSpawnDist);
		
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
