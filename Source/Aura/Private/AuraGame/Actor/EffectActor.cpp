// Copyright rynnli


#include "AuraGame/Actor/EffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/Public/RPGFramework/GAS/AttributeSets/RPGAttributeSetBase.h"

#include "GameplayEffect.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
	
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto& EffectClass : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto& EffectClass : DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto& EffectClass : InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto& EffectClass : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto& EffectClass : DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto& EffectClass : InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle>* HandlesToRemove = ActiveEffectHandles.Find(TargetASC);
		if (HandlesToRemove)
		{
			for (FActiveGameplayEffectHandle& Handle : *HandlesToRemove)
			{
				TargetASC->RemoveActiveGameplayEffect(Handle, 1);
			}
			ActiveEffectHandles.Remove(TargetASC);
		}
	}
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.FindOrAdd(TargetASC).Add(ActiveEffectHandle);
	}

	if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
}





