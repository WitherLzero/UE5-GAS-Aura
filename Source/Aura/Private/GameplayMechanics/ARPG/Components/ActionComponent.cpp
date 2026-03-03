// Copyright rynnli


#include "GameplayMechanics/ARPG/Components/ActionComponent.h"

#include "MotionWarpingComponent.h"


UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner) return;
	
	MotionWarpingComp = Owner->FindComponentByClass<UMotionWarpingComponent>();
	
	if (!MotionWarpingComp)
	{
		MotionWarpingComp = NewObject<UMotionWarpingComponent>(Owner);
		MotionWarpingComp->RegisterComponent();
	}
}

void UActionComponent::UpdateFacingTarget(const FVector& TargetLoc)
{
	MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(FacingTargetName, TargetLoc);
}



