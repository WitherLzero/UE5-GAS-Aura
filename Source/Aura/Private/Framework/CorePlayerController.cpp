// Copyright rynnli


#include "Framework/CorePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Framework/CoreCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "Input/RPGInputComponent.h"
#include "Interaction/EnemyInterface.h"


ACorePlayerController::ACorePlayerController()
{
	bReplicates = true;
}

void ACorePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}


void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(CurrentMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext( CurrentMappingContext,0);
	}
	
	UpdateMouse();
}

void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	URPGInputComponent* RPGInputComponent = CastChecked<URPGInputComponent>(InputComponent);
	
	RPGInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ACorePlayerController::Move);
	RPGInputComponent->BindTaggedAction(InputConfig,this,&ThisClass::OnInputTagPressed,&ThisClass::OnInputTagReleased,&ThisClass::OnInputTagHeld);
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

void ACorePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr) 
		{
			/* Last->null, This->valid 
			 * First trace, highlight the new enemy */
			ThisActor->HighlightActor();
		}
		/* Last/This->null 
		 * Do nothing */
	}
	else
	{
		if (ThisActor == nullptr) 
		{
			/* Last->valid, This->null 
			 * UnHighlight Last */
			LastActor->UnHighlightActor();
		}
		else
		{
			if (ThisActor != LastActor)
			{
				/* Last/This->valid, Last != This 
				* UnHighlight Last, Highlight This */
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			/* Last == This
			 * Do nothing */
		}
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

void ACorePlayerController::OnInputTagPressed(FGameplayTag InputTag)
{
	ProcessInputTag(InputTag,ERPGInputEvent::IE_Pressed);
}

void ACorePlayerController::OnInputTagReleased(FGameplayTag InputTag)
{
	ProcessInputTag(InputTag,ERPGInputEvent::IE_Released);
}

void ACorePlayerController::OnInputTagHeld(const FInputActionValue& InputActionValue,FGameplayTag InputTag)
{
	ProcessInputTag(InputTag,ERPGInputEvent::IE_Held,InputActionValue);
}

void ACorePlayerController::ProcessInputTag(FGameplayTag InputTag, ERPGInputEvent EventType,
                                            const FInputActionValue& InputActionValue)
{
	if (EventType == ERPGInputEvent::IE_Released) GetASC()->AbilityInputTagReleased(InputTag);
	if (EventType == ERPGInputEvent::IE_Held) GetASC()->AbilityInputTagHeld(InputTag);

}

UCoreAbilitySystemComponent* ACorePlayerController::GetASC()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}


