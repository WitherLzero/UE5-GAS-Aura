// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "VitalityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UVitalityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVitalityComponent();

	UFUNCTION(BlueprintCallable, Category = "Vitality")
	void InitVitality(UAbilitySystemComponent* ASC);
	
	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FOnDeathSignature OnDeath;
protected:
	virtual void BeginPlay() override;
	
	void Die();
	
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	void PerformRagdoll();
	
	bool bIsDead = false;
};
