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
			"EnhancedInput",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"ModularGameplayActors"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"CommonUI",
			"CommonGame",
			"CommonUser",
		});
	}
}
