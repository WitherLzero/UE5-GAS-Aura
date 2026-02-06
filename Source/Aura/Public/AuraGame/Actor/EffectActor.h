// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM()
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AEffectActor();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
 	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Instant")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Instant")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Duration")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Duration")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Infinite")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Infinite")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect | Infinite")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	float EffectLevel = 1.f;
	
	TMap<UAbilitySystemComponent*, TArray<FActiveGameplayEffectHandle>> ActiveEffectHandles;
	
private:
	
	
};
