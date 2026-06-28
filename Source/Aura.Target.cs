// Copyright rynnli

using UnrealBuildTool;
using System.Collections.Generic;

public class AuraTarget : TargetRules
{
	public AuraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Error;

		ExtraModuleNames.AddRange( new string[] { "Aura" } );
	}
}
