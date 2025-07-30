// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CryptRaid : ModuleRules
{
	public CryptRaid(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
