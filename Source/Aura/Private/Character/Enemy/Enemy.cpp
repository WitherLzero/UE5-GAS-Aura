// Copyright rynnli


#include "Character/Enemy/Enemy.h"

#include "Aura/Aura.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
}

void AEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

