// Copyright rynnli


#include "AuraGame/Character/AuraCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayMechanics/ARPG/Components/ActionComponent.h"
#include "GameplayMechanics/Core/Components/CombatComponent.h"
#include "GameplayMechanics/Core/Components/VitalityComponent.h"
#include "RPGFramework/Types/RPGGameplayTags.h"


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

}

void AAuraCharacterBase::InitComponentsWithASC(UAbilitySystemComponent* ASC)
{
	VitalityComp->InitVitality(ASC);
	VitalityComp->OnDeath.AddDynamic(this,&AAuraCharacterBase::HandleDeath);
	
	ActionComp->InitActionComponent(ASC);
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation() const
{
	return Weapon;
}

AActor* AAuraCharacterBase::GetCombatTarget_Implementation() const
{
	if (CombatComp)
	{
		return CombatComp->GetCombatTarget();
	}
	return nullptr;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
	if (CombatComp)
	{
		return CombatComp->GetCombatSocketLocation(MontageTag);
	}
	return FVector::ZeroVector;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	if (CombatComp)
	{
		return CombatComp->GetAttackMontages();
	}
	return TArray<FTaggedMontage>();
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation() const
{
	if (CombatComp)
	{
		return CombatComp->GetHitReactMontage();
	}
	return nullptr;
}

UNiagaraSystem* AAuraCharacterBase::GetHitReactEffect_Implementation() const
{
	if (CombatComp)
	{
		return CombatComp->GetHitReactEffect();
	}
	return nullptr;
}

void AAuraCharacterBase::SetCombatTarget_Implementation(AActor* InTarget)
{
	if (CombatComp)
	{
		CombatComp->SetCombatTarget(InTarget);
	}
}

void AAuraCharacterBase::HandleDeath(AActor* DeadActor, AActor* KillerActor)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	Dissolve();
	SetLifeSpan(LifeSpan);
}

void AAuraCharacterBase::SendDeathEvent(AActor* DeadActor)
{
	const FRPGGameplayTags& GameplayTags = FRPGGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.Target = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DeadActor,GameplayTags.Event_Character_Died,Payload);
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


