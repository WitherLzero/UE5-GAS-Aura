// Copyright rynnli

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "RPGInputConfig.h"
#include "RPGInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API URPGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URPGInputComponent();
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityAction(const URPGInputConfig* InputConfig,UserClass* Object,
		PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void URPGInputComponent::BindAbilityAction(const URPGInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	
	for (const FRPGInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (PressedFunc)
		{
			BindAction(Action.InputAction,ETriggerEvent::Started,PressedFunc,Action.InputTag);
		}
		if (ReleasedFunc)
		{
			BindAction(Action.InputAction,ETriggerEvent::Completed,ReleasedFunc,Action.InputTag);
		}
		if (HeldFunc)
		{
			BindAction(Action.InputAction,ETriggerEvent::Triggered,HeldFunc,Action.InputTag);
		}
		
	}
}
