// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "GxAbilityTask_PlayMontageAndWait.generated.h"

/**
 * UGxAbilityTask_PlayMontageAndWait
 */
UCLASS()
class UGxAbilityTask_PlayMontageAndWait : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="GxPlayMontageAndWait", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGxAbilityTask_PlayMontageAndWait* CreateGxPlayMontageAndWaitProxy(UGameplayAbility* GxOwningAbility,
			FName GxTaskInstanceName, UAnimMontage* GxMontageToPlay, FName GxStartSection = NAME_None, float GxRate = 1.f, bool bGxStopWhenAbilityEnds = true, float GxAnimRootMotionTranslationScale = 1.f, float GxStartTimeSeconds = 0.f);
	
	virtual void Activate() override;

protected:
	virtual void OnDestroy(bool AbilityEnded) override;
};
