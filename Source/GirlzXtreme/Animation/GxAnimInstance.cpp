// Fill out your copyright notice in the Description page of Project Settings.

#include "GxAnimInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAnimInstance)

//////////////////////////////////////////////////////////////////////////
// UGxAnimInstance

UGxAnimInstance::UGxAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsPlayingAbilityMontage(false)
{
}

void UGxAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UGxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	bIsPlayingAbilityMontage = (0 < NumAbilityAnimStack);
}
