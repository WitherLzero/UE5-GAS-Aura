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
	
	/* ICombatInterface */
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation() const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	/* end ICombatInterface */
	
	virtual void InitAbilityActorInfo();
	
	virtual void InitDefaultAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	
	void AddCharacterAbilities();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Input | Native")
	bool OnNativeInput(FGameplayTag Tag, ERPGInputEvent EventType, FInputActionValue Value);
	
	UPROPERTY(VisibleAnywhere, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category= "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
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
