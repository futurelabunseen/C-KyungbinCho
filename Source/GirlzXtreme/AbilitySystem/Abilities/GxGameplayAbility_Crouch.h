// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_Crouch.generated.h"

struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * UGxaGameplayAbility_Crouch
 *
 * 캐릭터 앉기 GA
 */
UCLASS(Abstract, Meta = (ShortTooltip = "Gameplay ability used for character Crouching."))
class UGxGameplayAbility_Crouch : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_Crouch(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

	UFUNCTION(BlueprintCallable, Category = "Gx|Ability")
	void CharacterCrouchStart();

	UFUNCTION(BlueprintCallable, Category = "Gx|Ability")
	void CharacterCrouchStop();
};
