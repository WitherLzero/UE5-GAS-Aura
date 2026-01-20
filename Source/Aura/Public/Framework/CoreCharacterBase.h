// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/InputInteractable.h"
#include "CoreCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API ACoreCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IInputInteractable
{
	GENERATED_BODY()

public:
	ACoreCharacterBase();

	virtual void Move(const FVector2D& InputAxis);
	
protected:
	virtual void BeginPlay() override;
	
	/* Input Interactable */
	virtual bool HandleNativeInput(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value) override;
	/* end Input Interactable*/
	
	virtual void InitAbilityActorInfo();
	
	void InitDefaultAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	
	void AddCharacterAbilities();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Input | Native")
	bool OnNativeInput(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value);
	
	UPROPERTY(VisibleAnywhere, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;	
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;	
	
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
};
