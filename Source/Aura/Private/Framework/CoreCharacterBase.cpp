// Copyright rynnli


#include "Framework/CoreCharacterBase.h"

ACoreCharacterBase::ACoreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
}

void ACoreCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


