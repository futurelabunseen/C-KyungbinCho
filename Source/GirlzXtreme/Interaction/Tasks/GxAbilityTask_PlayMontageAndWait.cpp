// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/Tasks/GxAbilityTask_PlayMontageAndWait.h"

#include "GxLogChannels.h"
#include "Character/GxCharacter.h"
#include "Animation/GxAnimInstance.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAbilityTask_PlayMontageAndWait)

//////////////////////////////////////////////////////////////////////////
// UGxAbilityTask_PlayMontageAndWait

UGxAbilityTask_PlayMontageAndWait::UGxAbilityTask_PlayMontageAndWait(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGxAbilityTask_PlayMontageAndWait* UGxAbilityTask_PlayMontageAndWait::CreateGxPlayMontageAndWaitProxy(UGameplayAbility* GxOwningAbility,
	FName GxTaskInstanceName, UAnimMontage* GxMontageToPlay, FName GxStartSection, float GxRate, bool bGxStopWhenAbilityEnds, float GxAnimRootMotionTranslationScale, float GxStartTimeSeconds)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(GxRate);

	UGxAbilityTask_PlayMontageAndWait* MyObj = NewAbilityTask<UGxAbilityTask_PlayMontageAndWait>(GxOwningAbility, GxTaskInstanceName);
	MyObj->MontageToPlay = GxMontageToPlay;
	MyObj->Rate = GxRate;
	MyObj->StartSection = GxStartSection;
	MyObj->AnimRootMotionTranslationScale = GxAnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bGxStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = GxStartTimeSeconds;

	return MyObj;
}


void UGxAbilityTask_PlayMontageAndWait::Activate()
{
	Super::Activate();

	AGxCharacter* GxCharacter = Cast<AGxCharacter>(Ability->GetAvatarActorFromActorInfo());
	gxcheck(GxCharacter);
	UGxAnimInstance* GxAnimInstance = GxCharacter->GetGxAnimInstance();
	gxcheck(GxAnimInstance);

	float startTime = 0, endTime = 0;
	GX_LOG(Warning, TEXT("%f"), MontageToPlay->GetSectionLength(0));
	MontageToPlay->GetSectionStartAndEndTime(0, startTime, endTime);
	GX_LOG(Warning, TEXT("%f %f"), startTime, endTime);

	GxAnimInstance->IncreaseAbilityAnimStack();
}

void UGxAbilityTask_PlayMontageAndWait::OnDestroy(bool AbilityEnded)
{
	AGxCharacter* GxCharacter = Cast<AGxCharacter>(Ability->GetAvatarActorFromActorInfo());
	gxcheck(GxCharacter);
	UGxAnimInstance* GxAnimInstance = GxCharacter->GetGxAnimInstance();
	gxcheck(GxAnimInstance);

	GxAnimInstance->DecreaseAbilityAnimStack();

	Super::OnDestroy(AbilityEnded);
}
