// Copyright rynnli


#include "AuraGame/Character/Enemy/Enemy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RPGFramework/Types/RPGGameplayTags.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/GAS/RPGAbilitySystemLibrary.h"
#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"
#include "RPGFramework/AI/RPGAIController.h"
#include "RPGFramework/UI/Widgets/UserWidgetBase.h"
#include "RPGModules/Components/CombatComponent.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "AuraGame/GAS/AttributeSets/CombatAttributeSet.h"
#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
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

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	AIController = Cast<ARPGAIController>(NewController);
	AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<URPGAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();

	if (HasAuthority())
	{
		InitDefaultAttributes();
		URPGAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent, CharacterClass);
	}
	
	Super::InitAbilityActorInfo();
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


void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AEnemy::HandleDeath(AActor* DeadActor, AActor* KillerActor)
{
	if (HasAuthority())
	{
    	SendXPEvent(KillerActor);
	}
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	}
	Super::HandleDeath(DeadActor, KillerActor);
	
}

void AEnemy::SendXPEvent(AActor* KillerActor)
{
	int32 XPReward = URPGAbilitySystemLibrary::GetXPRewardForClassAndLevel(this,CharacterClass,Level);
	const FRPGGameplayTags& GameplayTags = FRPGGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
	Payload.EventMagnitude = XPReward;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(KillerActor,GameplayTags.Attributes_Meta_IncomingXP,Payload);

}


