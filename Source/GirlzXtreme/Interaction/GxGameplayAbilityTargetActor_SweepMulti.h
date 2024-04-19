// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbilityTargetActor_Trace.h"

#include "GxGameplayAbilityTargetActor_SweepMulti.generated.h"

UCLASS(Blueprintable)
class AGxGameplayAbilityTargetActor_SweepMulti : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_UCLASS_BODY()

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};
