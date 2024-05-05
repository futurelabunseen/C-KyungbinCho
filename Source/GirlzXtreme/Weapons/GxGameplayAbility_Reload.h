// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_Reload.generated.h"

class UAnimMontage;

/**
 * UGxGameplayAbility_Reload
 *
 * 재장전 어빌리티다.
 */
UCLASS(Meta = (ShortTooltip = "Reload ability."))
class UGxGameplayAbility_Reload : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_Reload(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGxGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGxGameplayAbility interface

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gx|Reload")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UFUNCTION()
	void OnFinishedCallback();
	
	UFUNCTION()
	void OnCancelledCallback();
};
