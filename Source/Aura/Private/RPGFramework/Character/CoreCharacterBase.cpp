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


bool ACoreCharacterBase::OnNativeInput_Implementation(FGameplayTag Tag, ERPGInputEvent EventType,
                                                      FInputActionValue Value)
{
	return false;
}






