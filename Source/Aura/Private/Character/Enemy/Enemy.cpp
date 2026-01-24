// Copyright rynnli


#include "Character/Enemy/Enemy.h"

#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "AbilitySystem/RPGAbilitySystemLibrary.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "AbilitySystem/AttributeSets/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSets/VitalAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/UserWidgetBase.h"


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

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	InitAbilityActorInfo();

	if (UUserWidgetBase* HealthWidget = Cast<UUserWidgetBase>(HealthBar->GetUserWidgetObject()))
	{
		HealthWidget->SetWidgetController(this);
	}
	
	OnHealthChanged.Broadcast(VitalAS->GetHealth());
	OnMaxHealthChanged.Broadcast(VitalAS->GetMaxHealth());
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		VitalAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			VitalAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data){ OnMaxHealthChanged.Broadcast(Data.NewValue);});
	
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UCoreAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();
	
	InitDefaultAttributes();
}

void AEnemy::InitDefaultAttributes() const
{
	URPGAbilitySystemLibrary::InitDefaultAttributes(this,GetAbilitySystemComponent(),CharacterClass,Level);
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

