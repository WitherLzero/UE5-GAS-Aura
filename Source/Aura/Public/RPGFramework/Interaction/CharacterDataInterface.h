// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCharacterDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICharacterDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Die() = 0;
	
	virtual int32 GetCharacterLevel() const;
};
