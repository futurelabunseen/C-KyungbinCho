// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GirlzXtreme : ModuleRules
{
	public GirlzXtreme(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"GirlzXtreme"
			}
		);

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"PhysicsCore",
			"EnhancedInput",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"ModularGameplay",
			"ModularGameplayActors"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"CommonUI",
			"CommonGame",
			"CommonUser",
		});
	}
}
