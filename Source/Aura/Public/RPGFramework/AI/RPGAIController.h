// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "RPGAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class AURA_API ARPGAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARPGAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;


};
