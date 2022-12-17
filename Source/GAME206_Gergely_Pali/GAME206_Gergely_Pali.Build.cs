// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAME206_Gergely_Pali : ModuleRules
{
	public GAME206_Gergely_Pali(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore" });
	}
}
