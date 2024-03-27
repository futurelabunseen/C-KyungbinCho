// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayAbility_Jump.h"

#include "GxLogChannels.h"
#include "Character/GxCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbility_Jump)

//////////////////////////////////////////////////////////////////////////
// UGxGameplayAbility_Jump

UGxGameplayAbility_Jump::UGxGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGxGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	gxcheck(ActorInfo, false);
	gxcheck(ActorInfo->AvatarActor.IsValid(), false);

	const AGxCharacter* GxCharacter = Cast<AGxCharacter>(ActorInfo->AvatarActor.Get());
	gxcheck(GxCharacter, false);

	return GxCharacter->CanJump();
}

void UGxGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);

	CharacterJumpStart();
}

void UGxGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_Jump::CharacterJumpStart()
{
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);

	if (GxCharacter->IsLocallyControlled())
	{
		GxCharacter->UnCrouch();
		GxCharacter->Jump();
		GxCharacter->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	}
}

void UGxGameplayAbility_Jump::CharacterJumpStop()
{
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);

	if (GxCharacter->IsLocallyControlled())
	{
		GxCharacter->StopJumping();
		GxCharacter->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);
	}
}

void UGxGameplayAbility_Jump::OnLandedCallback(const FHitResult& Hit)
{
	K2_EndAbility();
}
