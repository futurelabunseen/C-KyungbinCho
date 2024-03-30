// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayAbility_FireSemiAuto.h"

#include "GxLogChannels.h"
#include "DrawDebugHelpers.h"
#include "GxGameplayTags.h"
#include "Player/GxPlayerController.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbility_FireSemiAuto)

//////////////////////////////////////////////////////////////////////////
// UGxGameplayAbility_FireSemiAuto

UGxGameplayAbility_FireSemiAuto::UGxGameplayAbility_FireSemiAuto(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGxGameplayAbility_FireSemiAuto::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::Instant, TargetActorClass);
	gxcheck(WaitTargetDataTask);
	
	WaitTargetDataTask->ValidData.AddDynamic(this, &ThisClass::ValidDataCallback);
	WaitTargetDataTask->Cancelled.AddDynamic(this, &ThisClass::CancelledCallback);

	AGameplayAbilityTargetActor* SpawnedActor = nullptr;
	WaitTargetDataTask->BeginSpawningActor(this, TargetActorClass, SpawnedActor);
	gxcheck(SpawnedActor);

	AActor* SourceActor = GetAvatarActorFromActorInfo();
	gxcheck(SourceActor);

	FGameplayAbilityTargetingLocationInfo GameplayAbilityTargetingLocationInfo;
	GameplayAbilityTargetingLocationInfo.SourceActor = SourceActor;
	GameplayAbilityTargetingLocationInfo.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
	SpawnedActor->StartLocation = GameplayAbilityTargetingLocationInfo;

	WaitTargetDataTask->FinishSpawningActor(this, SpawnedActor);
	WaitTargetDataTask->ReadyForActivation();
}

void UGxGameplayAbility_FireSemiAuto::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_FireSemiAuto::ValidDataCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
			{
				// [TODO] 피격 처리
				UE_DEBUG_BREAK();
				FGameplayEventData Payload;
				AActor* Instigator = GetAvatarActorFromActorInfo();
				gxcheck(Instigator);
				Payload.Instigator = Instigator;
				Payload.EventMagnitude = Damage;
				Payload.EventTag = GxGameplayTags::SetByCaller_Damage;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, GxGameplayTags::SetByCaller_Damage, Payload);
				GX_LOG(Warning, TEXT("SetByCaller.Damage ==> %s ..."), *(HitResult.GetActor()->GetName()));
			}

			GX_LOG(Warning, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
#if ENABLE_DRAW_DEBUG
			AGxPlayerController* GxPC = GetGxPlayerControllerFromActorInfo();
			gxcheck(GxPC);
			FVector ViewLoc;
			FRotator ViewRot;
			GxPC->GetPlayerViewPoint(ViewLoc, ViewRot);

			DrawDebugLine(GetWorld(), ViewLoc, HitResult.ImpactPoint, FColor::Green, /*bPersistentLines*/false, /*LifeTime*/3.0f, /*DepthPriority*/0, /*Thickness*/0.5f);
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, /*Radius*/15.0f, /*Segments*/8, FColor::Red, /*bPersistentLines*/false, /*LifeTime*/3.0f, /*DepthPriority*/0, /*Thickness*/0.5f);
#endif
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/false);
}

void UGxGameplayAbility_FireSemiAuto::CancelledCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/true);
}
