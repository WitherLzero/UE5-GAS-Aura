// Copyright rynnli


#include "Character/Enemy/Enemy.h"

#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "AbilitySystem/AttributeSets/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSets/VitalAttributeSet.h"
#include "Aura/Aura.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UCoreAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	VitalAS = CreateDefaultSubobject<UVitalAttributeSet>("VitalAttributeSet");
	PrimaryAS = CreateDefaultSubobject<UPrimaryAttributeSet>("PrimaryAttributeSet");
	CombatAS = CreateDefaultSubobject<UCombatAttributeSet>("CombatAttributeSet");
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	InitAbilityActorInfo();
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UCoreAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();
	
	InitDefaultAttributes();
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

int32 AEnemy::GetCharacterLevel() const
{
	return Level;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

