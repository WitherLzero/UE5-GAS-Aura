// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
struct FGameplayEffectSpecHandle;
class ARPGProjectile;
class UAbilityInfo;
struct FTaggedMontage;
class UCharacterClassInfo;
class UGameplayEffect;
struct FGameplayEffectContextHandle;
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

	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|CharacterClassDefaults")
	static void InitDefaultAttributes(const UObject* WorldContextObject,UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level);

	UFUNCTION(BlueprintCallable, Category="RPGAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(Category="RPGAbilitySystemLibrary|CharacterClassDefaults")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
	UFUNCTION(BlueprintCallable, Category="RPGAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="RPGAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	
	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintCallable,Category = "RPGAbilitySystemLibrary|GameplayMechanics|Projectile", meta = (DefaultToSelf = "WorldContextObject"))
	static ARPGProjectile* SpawnProjectileWithDamage(const UObject* WorldContextObject,TSubclassOf<ARPGProjectile> ProjectileClass,
													const FVector& SpawnLocation, const FVector& TargetLocation, 
													AActor* Instigator, const FDamageEffectParams& DamageEffectParams);
	
	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectSpecHandle MakeDamageEffectSpec(const FDamageEffectParams& DamageEffectParams);
	
	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayHelpers")
	static FTaggedMontage PickRandomTaggedMontage (const TArray<FTaggedMontage>& Montages);
	
private:
	static void ApplyEffectToSelf(UAbilitySystemComponent* ASC, AActor* Avatar,
							  TSubclassOf<UGameplayEffect> EffectClass,
							  float Level);

};


