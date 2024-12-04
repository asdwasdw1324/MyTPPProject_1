// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyTPPProject : ModuleRules
{
	public MyTPPProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"AIModule", 
			"GameplayTasks", 
			"UMG", 
			"GameplayTags", 
			"GameplayAbilities" });
	}
}
