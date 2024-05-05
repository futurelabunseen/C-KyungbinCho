// Copyright Epic Games, Inc. All Rights Reserved.

#include "GxGameplayAbilityTargetActor_SweepMulti.h"

#include "GxLogChannels.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Abilities/GameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbilityTargetActor_SweepMulti)

AGxGameplayAbilityTargetActor_SweepMulti::AGxGameplayAbilityTargetActor_SweepMulti(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FHitResult AGxGameplayAbilityTargetActor_SweepMulti::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_SingleLineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();// InSourceActor->GetActorLocation();
	FVector TraceEnd;
	AimWithPlayerController(InSourceActor, Params, TraceStart, TraceEnd);		//Effective on server and launching client only

	APlayerController* PC = OwningAbility->GetCurrentActorInfo()->PlayerController.Get();
	gxcheck_log(PC);

	FVector ViewStart;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewStart, ViewRot);
	
	// ------------------------------------------------------

	FHitResult ReturnHitResult;
	GX_NET_LOG_SCREEN(FColor::White, TEXT("Start: %s"), *ViewStart.ToString());
	GX_NET_LOG_SCREEN(FColor::White, TEXT("End: %s"), *(ViewStart + (ViewRot.Vector() * 400.f)).ToString());
	SweepWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, ViewStart, ViewStart + (ViewRot.Vector() * 400.f), ViewRot.Quaternion(), FCollisionShape::MakeCapsule(50.0f, 50.0f), TraceProfile.Name, Params);

#if ENABLE_DRAW_DEBUG
	GX_NET_LOG_SCREEN(FColor::White, TEXT("Sphere: %s"), *(ViewStart + (ViewRot.Vector() * 400.f)).ToString());
	//DrawDebugCapsule(GetWorld(), (ViewStart + (ViewStart + (ViewRot.Vector() * 400.f))) / 2.f, 200.0f, 16, FColor::Green);
#endif
	return ReturnHitResult;
}
