// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_BioticGrenade.generated.h"

class AGxCharacter;
class AGxWeapon;
class UAnimMontage;

/**
 * UGxGameplayAbility_BioticGrenade
 *
 * 지원A 생체 수류탄 스킬
 */
UCLASS(Abstract, Meta = (ShortTooltip = "Gameplay ability used for HealA hero biotic grenade skill."))
class UGxGameplayAbility_BioticGrenade : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_BioticGrenade(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGxGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGxGameplayAbility interface

	UFUNCTION()
	void OnFinishedCallback();
	
	UFUNCTION()
	void OnCancelledCallback();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gx|SwiftStrike")
	TSubclassOf<AGxCharacter> ActivatableHeroClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gx|SwiftStrike")
	UAnimMontage* AnimMontage;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|SwiftStrike")
	TObjectPtr<AGxWeapon> CurrentWeapon;
};
