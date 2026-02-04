// Copyright rynnli


#include "RPGFramework/Character/CoreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "InputActionValue.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

ACoreCharacterBase::ACoreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
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

void ACoreCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}


void ACoreCharacterBase::InitAbilityActorInfo()
{
}

void ACoreCharacterBase::InitDefaultAttributes() const
{
}

void ACoreCharacterBase::AddCharacterAbilities()
{
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



void ACoreCharacterBase::Dissolve()
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

void ACoreCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

bool ACoreCharacterBase::OnNativeInput_Implementation(FGameplayTag Tag, ERPGInputEvent EventType,
                                                      FInputActionValue Value)
{
	return false;
}






