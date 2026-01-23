// Copyright rynnli


#include "Character/Player/AuraCharacter.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Framework/CorePlayerController.h"
#include "Framework/CorePlayerState.h"
#include "Input/RPGInputConfig.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AutoRun();

}

int32 AAuraCharacter::GetCharacterLevel() const
{
	ACorePlayerState* AuraPlayerState = GetPlayerState<ACorePlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

bool AAuraCharacter::HandleNativeInput(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value)
{
	return OnNativeInput(Tag,EventType,Value);
}


bool AAuraCharacter::OnNativeInput_Implementation(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value)
{
	if (Tag == FRPGGameplayTags::Get().Inputs_Move)
	{
		if (EventType == ERPGInputEvent::IE_Held)
		{
			const FVector2D InputAxis = Value.Get<FVector2D>();
			Move(InputAxis);
			return true;
		}
	}
	if (Tag == FRPGGameplayTags::Get().Inputs_LMB)
	{
		if (EventType == ERPGInputEvent::IE_Pressed)
		{
			bTargeting = GetPC()->HitEnemyActor();
			bAutoRunning = false;
			return true;
		}
		if (EventType == ERPGInputEvent::IE_Held)
		{
			if (HoldToMove()) return true;
		}
		if (EventType == ERPGInputEvent::IE_Released)
		{
			if (SetupNavPoints()) return true;
		}
	}
	
	return false;
}


// Init ability actor info for the Server
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

// Init ability actor info for the Client
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	ACorePlayerState* AuraPlayerState = GetPlayerState<ACorePlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UCoreAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();
	
	InitDefaultAttributes();
	if (ACorePlayerController* AuraPlayerController = Cast<ACorePlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AbilitySystemComponent,AuraPlayerController,AuraPlayerState);
		}
	}
}

ACorePlayerController* AAuraCharacter::GetPC()
{
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<ACorePlayerController>(GetController());
	}
	return PlayerController;
}

void AAuraCharacter::Move(const FVector2D& InputAxis)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Forward,InputAxis.Y);
	AddMovementInput(Right,InputAxis.X);
}

bool AAuraCharacter::HoldToMove()
{
	if (!bTargeting)
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if (GetPC()->GetCursorHit(Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
				
		const FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
		AddMovementInput(WorldDirection);
		return true;
	}
	return false;
}

bool AAuraCharacter::SetupNavPoints()
{
	if (!bTargeting)
	{
		if (FollowTime <= ShortPressThreshold)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this,GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc: NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			}
			bAutoRunning = true;
		}
		FollowTime = 0.f;
		bTargeting = false;
		return true;
	}
	return false;
}

void AAuraCharacter::AutoRun()
{
	if (bAutoRunning)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		AddMovementInput(Direction);
		
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

