// Copyright rynnli


#include "RPGModules/Components/VitalityComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"


// Sets default values for this component's properties
UVitalityComponent::UVitalityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UVitalityComponent::InitVitality(UAbilitySystemComponent* ASC)
{
	if (IsValid(ASC))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UVitalAttributeSet::GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				if (!bIsDead && Data.NewValue <= 0.f)
				{
					Die();
				}
			});
	}
}



void UVitalityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UVitalityComponent::Die()
{
	bIsDead = true;
	OnDeath.Broadcast(GetOwner());
	MulticastHandleDeath();
}



void UVitalityComponent::MulticastHandleDeath_Implementation()
{
	PerformRagdoll();
}

void UVitalityComponent::PerformRagdoll()
{
	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		if (UCapsuleComponent* Capsule = Owner->GetCapsuleComponent())
		{
			Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (USkeletalMeshComponent* Mesh = Owner->GetMesh())
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->SetEnableGravity(true);
			Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		}
	}
}



