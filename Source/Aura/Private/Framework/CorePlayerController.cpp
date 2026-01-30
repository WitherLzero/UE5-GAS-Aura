// Copyright rynnli


#include "Framework/CorePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Framework/CoreCharacterBase.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CoreAbilitySystemComponent.h"
#include "Input/RPGInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"


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
	
	RPGInputComponent->BindTaggedAction(InputConfig,this,&ThisClass::OnInputTagPressed,&ThisClass::OnInputTagReleased,&ThisClass::OnInputTagHeld);
}

void ACorePlayerController::ShowDamageNumber_Implementation(ACharacter* TargetCharacter, float DamageAmount, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextCompClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextCompClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount,bBlockedHit,bCriticalHit);
	}
}

void ACorePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
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
	if (IInputInteractable* Interface = Cast<IInputInteractable>(GetPawn()))
	{
		if (Interface->HandleNativeInput(InputTag,EventType,InputActionValue)) return;
	}

	if (GetASC())
	{
		if (EventType == ERPGInputEvent::IE_Released) GetASC()->AbilityInputTagReleased(InputTag);
		if (EventType == ERPGInputEvent::IE_Held) GetASC()->AbilityInputTagHeld(InputTag);
	}

}

UCoreAbilitySystemComponent* ACorePlayerController::GetASC()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}

bool ACorePlayerController::GetCursorHit(FHitResult& HitResult)
{
	return GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
}


