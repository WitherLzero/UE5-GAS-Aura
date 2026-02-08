// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"


class UMotionWarpingComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionComponent();

	UFUNCTION(BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLoc);
protected:
	virtual void BeginPlay() override;

	TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;
	
	UPROPERTY(EditAnywhere, Category= "ARPG")
	FName FacingTargetName;
};
