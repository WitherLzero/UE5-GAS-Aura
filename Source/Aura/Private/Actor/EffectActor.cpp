// Copyright rynnli


#include "Actor/EffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CoreAttributeSet.h"


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
	
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddSourceObject(this); 	
	const FGameplayEffectSpecHandle EffectSpec = TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContext);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

}





