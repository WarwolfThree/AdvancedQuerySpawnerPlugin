// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EnvSpawner : ModuleRules
{
	public EnvSpawner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AQS" });
	}
}
