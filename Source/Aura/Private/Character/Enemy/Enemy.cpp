// Copyright rynnli


#include "Character/Enemy/Enemy.h"

#include "RPGGameplayTags.h"
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
	
	// Wait one tick to ensure InitAbilityActorInfo is fully settled and Replication potentially processed.
	// This solves the race condition where BeginPlay runs before attributes are replicated, catching up with the correct values.
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		if (IsValid(this) && IsValid(AbilitySystemComponent) && IsValid(VitalAS))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				VitalAS->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue);});
			
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
					VitalAS->GetMaxHealthAttribute()).AddLambda(
					[this](const FOnAttributeChangeData& Data){ OnMaxHealthChanged.Broadcast(Data.NewValue);});
			
			AbilitySystemComponent->RegisterGameplayTagEvent(FRPGGameplayTags::Get().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
				this,&ThisClass::HitReactTagChanged);

			// Broadcast initial values AFTER binding, ensuring we catch the latest replicated data
			OnHealthChanged.Broadcast(VitalAS->GetHealth());
			OnMaxHealthChanged.Broadcast(VitalAS->GetMaxHealth());
		}
	});
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UCoreAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();

	if (HasAuthority())
	{
		InitDefaultAttributes();
		URPGAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent);
	}
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

void AEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

