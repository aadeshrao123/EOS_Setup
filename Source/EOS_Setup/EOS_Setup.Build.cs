// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EOS_Setup : ModuleRules
{
	public EOS_Setup(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemEOS" });
	}
}
