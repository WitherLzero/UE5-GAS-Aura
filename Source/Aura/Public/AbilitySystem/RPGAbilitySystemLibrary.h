// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGAbilitySystemLibrary.generated.h"

enum class ECharacterClass : uint8;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API URPGAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystem|CharacterClassDefaults")
	static void InitDefaultAttributes(const UObject* WorldContextObject,UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
private:
	static void ApplyEffectToSelf(UAbilitySystemComponent* ASC, AActor* Avatar,
							  TSubclassOf<UGameplayEffect> EffectClass,
							  float Level);

};
