// Copyright rynnli


#include "AuraGame/Character/AuraCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "RPGModules/ARPG/Components/ActionComponent.h"
#include "RPGModules/Components/CombatComponent.h"
#include "RPGModules/Components/VitalityComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	VitalityComp = CreateDefaultSubobject<UVitalityComponent>("VitalityComp");
	CombatComp = CreateDefaultSubobject<UCombatComponent>("CombatComp");
	ActionComp = CreateDefaultSubobject<UActionComponent>("ActionComp");
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	CombatComp->RegisterWeaponMesh(Weapon);
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	VitalityComp->InitVitality(AbilitySystemComponent);
	VitalityComp->OnDeath.AddDynamic(this,&AAuraCharacterBase::OnDeathCallbacks);
}

void AAuraCharacterBase::OnDeathCallbacks(AActor* DeadActor)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	Dissolve();
	SetLifeSpan(LifeSpan);
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}


void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}


