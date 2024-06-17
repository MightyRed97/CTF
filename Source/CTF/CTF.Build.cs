// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CTF : ModuleRules
{
	public CTF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(
            new string[] {
                "CTF"
            }
        );
    }
}
