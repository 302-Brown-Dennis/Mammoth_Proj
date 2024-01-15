// Based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-5-cpp-multiplayer-shooter/
// Modified by: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMultiplayerSubSysPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
