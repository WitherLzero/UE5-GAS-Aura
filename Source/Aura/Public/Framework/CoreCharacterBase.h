// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoreCharacterBase.generated.h"

UCLASS()
class AURA_API ACoreCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACoreCharacterBase();

	virtual void Move(const FVector2D& InputAxis);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;


};
