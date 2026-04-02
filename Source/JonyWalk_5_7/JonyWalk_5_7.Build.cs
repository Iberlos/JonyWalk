// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JonyWalk_5_7 : ModuleRules
{
	public JonyWalk_5_7(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"JonyWalk_5_7",
			"JonyWalk_5_7/Variant_Platforming",
			"JonyWalk_5_7/Variant_Platforming/Animation",
			"JonyWalk_5_7/Variant_Combat",
			"JonyWalk_5_7/Variant_Combat/AI",
			"JonyWalk_5_7/Variant_Combat/Animation",
			"JonyWalk_5_7/Variant_Combat/Gameplay",
			"JonyWalk_5_7/Variant_Combat/Interfaces",
			"JonyWalk_5_7/Variant_Combat/UI",
			"JonyWalk_5_7/Variant_SideScrolling",
			"JonyWalk_5_7/Variant_SideScrolling/AI",
			"JonyWalk_5_7/Variant_SideScrolling/Gameplay",
			"JonyWalk_5_7/Variant_SideScrolling/Interfaces",
			"JonyWalk_5_7/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
