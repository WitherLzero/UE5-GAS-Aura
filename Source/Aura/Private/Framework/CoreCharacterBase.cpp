// Copyright rynnli


#include "Framework/CoreCharacterBase.h"

#include "AbilitySystemComponent.h"

ACoreCharacterBase::ACoreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
}

void ACoreCharacterBase::Move(const FVector2D& InputAxis)
{
}

void ACoreCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoreCharacterBase::InitAbilityActorInfo()
{
}

void ACoreCharacterBase::InitPrimaryAttributes() const
{
	if (!GetAbilitySystemComponent()) return;
	check(DefaultPrimaryAttributes);
	
	const FGameplayEffectContextHandle ContextHandle =GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes,1.f,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}


