// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPGFramework/Interaction/CharacterDataInterface.h"
#include "RPGFramework/Interaction/InputInteractable.h"
#include "CoreCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API ACoreCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICharacterDataInterface, public IInputInteractable
{
	GENERATED_BODY()

public:
	ACoreCharacterBase();

	virtual void Move(const FVector2D& InputAxis);
	
protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo();
	virtual void InitDefaultAttributes() const;
	virtual void AddCharacterAbilities();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	
	
	virtual void Die() override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Input | Native")
	bool OnNativeInput(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value);
	
	
	UPROPERTY(VisibleAnywhere, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributes;	
	
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;
	
	
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
};
