// Copyright rynnli


#include "Framework/CorePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Framework/CoreCharacterBase.h"

ACorePlayerController::ACorePlayerController()
{
	bReplicates = true;
}



void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(CurrentMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext( CurrentMappingContext,0);
	
	UpdateMouse();
}

void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ACorePlayerController::Move);
}

void ACorePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxis = InputActionValue.Get<FVector2D>();

	// TODO: Use interface in the future 
	if (auto* ControlledPawn = GetPawn<ACoreCharacterBase>())
	{
		ControlledPawn->Move(InputAxis);
	}

}


void ACorePlayerController::UpdateMouse()
{
	// TODO: Specific cursor setting for Top-Down? Need to make it changeable 
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}