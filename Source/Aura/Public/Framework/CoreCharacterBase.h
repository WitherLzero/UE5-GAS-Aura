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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;


};
