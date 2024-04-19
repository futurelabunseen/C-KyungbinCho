// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayAbility_BioticGrenade.h"

#include "GxLogChannels.h"
#include "Interaction/Tasks/GxAbilityTask_PlayMontageAndWait.h"
#include "Character/GxCharacter.h"
#include "Weapons/GxWeapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbility_BioticGrenade)

//////////////////////////////////////////////////////////////////////////
// UGxGameplayAbility_BioticGrenade

UGxGameplayAbility_BioticGrenade::UGxGameplayAbility_BioticGrenade(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGxGameplayAbility_BioticGrenade::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter, false);

	return GxCharacter->IsA(ActivatableHeroClass);
}

void UGxGameplayAbility_BioticGrenade::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);
	CurrentWeapon = GxCharacter->GetCurrentWeapon();
	gxcheck(CurrentWeapon);
	GxCharacter->DisableWeapon(CurrentWeapon);

	UGxAbilityTask_PlayMontageAndWait* GxPlayMontageAndWaitTask = UGxAbilityTask_PlayMontageAndWait::CreateGxPlayMontageAndWaitProxy(this, NAME_None, AnimMontage);
	gxcheck(GxPlayMontageAndWaitTask);

	GxPlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnFinishedCallback);
	GxPlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnCancelledCallback);
	GxPlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledCallback);
	GxPlayMontageAndWaitTask->ReadyForActivation();
}

void UGxGameplayAbility_BioticGrenade::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);
	gxcheck(CurrentWeapon);
	GxCharacter->EnableWeapon(CurrentWeapon->GetClass());

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_BioticGrenade::OnFinishedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/false);
}

void UGxGameplayAbility_BioticGrenade::OnCancelledCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/true);
}
