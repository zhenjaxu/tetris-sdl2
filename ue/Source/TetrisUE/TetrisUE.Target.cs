using UnrealBuildTool;
using System.Collections.Generic;

public class TetrisUETarget : TargetRules
{
	public TetrisUETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "TetrisUE" });
	}
}
