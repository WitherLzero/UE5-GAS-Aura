// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACorePlayerController();
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputMappingContext> CurrentMappingContext;
	
	
};
