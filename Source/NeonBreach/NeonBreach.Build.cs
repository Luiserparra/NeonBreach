// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NeonBreach : ModuleRules
{
	public NeonBreach(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"SlateCore",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"NavigationSystem",
			"Niagara",
			"OnlineSubsystem",
			"OnlineSubsystemUtils"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"NeonBreach",
			"NeonBreach/Character",
			"NeonBreach/Player",
			"NeonBreach/Game",
			"NeonBreach/GAS",
			"NeonBreach/AI",
			"NeonBreach/Weapon",
			"NeonBreach/UI",
			"NeonBreach/Variant_Platforming",
			"NeonBreach/Variant_Platforming/Animation",
			"NeonBreach/Variant_Combat",
			"NeonBreach/Variant_Combat/AI",
			"NeonBreach/Variant_Combat/Animation",
			"NeonBreach/Variant_Combat/Gameplay",
			"NeonBreach/Variant_Combat/Interfaces",
			"NeonBreach/Variant_Combat/UI",
			"NeonBreach/Variant_SideScrolling",
			"NeonBreach/Variant_SideScrolling/AI",
			"NeonBreach/Variant_SideScrolling/Gameplay",
			"NeonBreach/Variant_SideScrolling/Interfaces",
			"NeonBreach/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
