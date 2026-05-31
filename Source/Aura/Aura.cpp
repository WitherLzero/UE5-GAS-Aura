// Copyright rynnli

#include "Aura.h"
#include "Modules/ModuleManager.h"
#include "AuraGame/Types/AuraGameplayTags.h"

class FAuraModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();
		
		FAuraGameplayTags::InitializeNativeGameplayTags();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FAuraModule, Aura, "Aura" );
