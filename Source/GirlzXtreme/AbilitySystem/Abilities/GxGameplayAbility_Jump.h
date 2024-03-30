// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_Jump.generated.h"

/**
 * UGxaGameplayAbility_Jump
 *
 * 캐릭터 점프 GA
 */
UCLASS(Abstract, Meta = (ShortTooltip = "Gameplay ability used for character jumping."))
class UGxGameplayAbility_Jump : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility interface
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UGameplayAbility interface

	//~UGxGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGxGameplayAbility interface

	UFUNCTION(BlueprintCallable, Category = "Gx|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Gx|Ability")
	void CharacterJumpStop();

	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
