// Copyright rynnli


#include "Framework/CoreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CoreAbilitySystemComponent.h"

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

void ACoreCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}

void ACoreCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	if (!GetAbilitySystemComponent()) return;
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle =GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACoreCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;
	
	CastChecked<UCoreAbilitySystemComponent>(AbilitySystemComponent)->AddCharacterAbilities(CharacterAbilities);
}


