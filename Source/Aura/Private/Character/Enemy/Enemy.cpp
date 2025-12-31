// Copyright rynnli


#include "Character/Enemy/Enemy.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::HighlightActor()
{
	bHighlighted = true;
}

void AEnemy::UnHighlightActor()
{
	bHighlighted = false;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

