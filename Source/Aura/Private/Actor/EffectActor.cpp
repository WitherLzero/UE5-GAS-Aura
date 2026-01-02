// Copyright rynnli


#include "Actor/EffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/CoreAttributeSet.h"
#include "Components/SphereComponent.h"


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AEffectActor::OnOverlap);
	
}

void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Need to change this to apply Gameplay Effect. 
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UCoreAttributeSet* CoreAttributeSet = Cast<UCoreAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UCoreAttributeSet::StaticClass()));
		UCoreAttributeSet* MutableCAS = const_cast<UCoreAttributeSet*>(CoreAttributeSet);
		MutableCAS->SetHealth(CoreAttributeSet->GetHealth() + 20.f);
		
		Destroy();
	
	}
}

