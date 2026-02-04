#pragma once

#include "GameplayEffectTypes.h"
#include "RPGAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FRPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FRPGGameplayEffectContext* Duplicate() const
	{
		FRPGGameplayEffectContext* NewContext = new FRPGGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
protected:
	
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit () const { return bIsBlockedHit; }
	
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
};


template<>
struct TStructOpsTypeTraits< FRPGGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FRPGGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};