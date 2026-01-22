// Copyright rynnli


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Kismet/GameplayStatics.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwingAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwingAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	ValidData.Broadcast(CursorHit.Location);
}
