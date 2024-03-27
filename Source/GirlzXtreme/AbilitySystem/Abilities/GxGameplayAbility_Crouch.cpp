// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayAbility_Crouch.h"

#include "GxLogChannels.h"
#include "GxGameplayTags.h"
#include "Character/GxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbility_Crouch)

//////////////////////////////////////////////////////////////////////////
// UGxGameplayAbility_Crouch

UGxGameplayAbility_Crouch::UGxGameplayAbility_Crouch(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGxGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	gxcheck(ActorInfo, false);
	gxcheck(ActorInfo->AvatarActor.IsValid(), false);

	const AGxCharacter* GxCharacter = Cast<AGxCharacter>(ActorInfo->AvatarActor.Get());
	gxcheck(GxCharacter, false);

	return GxCharacter->CanCrouch();
}

void UGxGameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);

	CharacterCrouchStart();
}

void UGxGameplayAbility_Crouch::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop Crouching in case the ability blueprint doesn't call it.
	CharacterCrouchStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_Crouch::CharacterCrouchStart()
{
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);

	if (GxCharacter->IsLocallyControlled())
	{
		GxCharacter->Crouch();
	}
}

void UGxGameplayAbility_Crouch::CharacterCrouchStop()
{
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);

	if (GxCharacter->IsLocallyControlled())
	{
		GxCharacter->UnCrouch();
	}
}
