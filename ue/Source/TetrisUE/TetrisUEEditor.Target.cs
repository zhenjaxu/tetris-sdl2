using UnrealBuildTool;
using System.Collections.Generic;

public class TetrisUEEditorTarget : TargetRules
{
	public TetrisUEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "TetrisUE" });
	}
}
