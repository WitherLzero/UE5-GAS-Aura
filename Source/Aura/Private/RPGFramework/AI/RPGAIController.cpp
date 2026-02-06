// Copyright rynnli


#include "RPGFramework/AI/RPGAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
ARPGAIController::ARPGAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
}


