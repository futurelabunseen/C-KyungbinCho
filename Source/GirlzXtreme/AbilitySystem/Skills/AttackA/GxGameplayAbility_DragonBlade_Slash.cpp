// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayAbility_DragonBlade_Slash.h"

#include "GxLogChannels.h"
#include "Interaction/Tasks/GxAbilityTask_PlayMontageAndWait.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbility_DragonBlade_Slash)

//////////////////////////////////////////////////////////////////////////
// UGxGameplayAbility_DragonBlade_Slash

UGxGameplayAbility_DragonBlade_Slash::UGxGameplayAbility_DragonBlade_Slash(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGxGameplayAbility_DragonBlade_Slash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UGxAbilityTask_PlayMontageAndWait* GxPlayMontageAndWaitTask = UGxAbilityTask_PlayMontageAndWait::CreateGxPlayMontageAndWaitProxy(this, NAME_None, AnimMontage, *FString::Printf(TEXT("Slash%d"), 1 + (CurrentSlashSection = ++CurrentSlashSection % 3)));
	gxcheck(GxPlayMontageAndWaitTask);

	GxPlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnFinishedCallback);
	GxPlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnCancelledCallback);
	GxPlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledCallback);
	GxPlayMontageAndWaitTask->ReadyForActivation();
}

void UGxGameplayAbility_DragonBlade_Slash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_DragonBlade_Slash::OnFinishedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/false);
}

void UGxGameplayAbility_DragonBlade_Slash::OnCancelledCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/true);
}
