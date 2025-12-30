// Copyright rynnli


#include "Framework/CorePlayerController.h"
#include "EnhancedInputSubsystems.h"

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
	
	// TODO: Specific cursor setting for Top-Down? Need to make it changeable 
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
