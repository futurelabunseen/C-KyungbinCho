// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_DragonBlade_Slash.generated.h"

class UAnimMontage;

/**
 * UGxGameplayAbility_DragonBlade_Slash
 *
 * 공격A 궁극기 용검의 베기 어빌리티다.
 * 용검 어빌리티에 의해 트리거된다.
 * 
 * 타격 판정은 몽타주의 노티파이에서 수행된다.
 * 베기 간격은 몽타주의 재생 시간으로 관리된다.
 */
UCLASS(Meta = (ShortTooltip = "AttackA hero dragon blade slash ability. Triggered by dragon blade ability."))
class UGxGameplayAbility_DragonBlade_Slash : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_DragonBlade_Slash(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGxGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGxGameplayAbility interface

	UPROPERTY(EditDefaultsOnly, Category = "Gx|DragonBlade|Slash")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|DragonBlade|Slash")
	float Damage;
	
protected:
	UFUNCTION()
	void OnFinishedCallback();

	UFUNCTION()
	void OnCancelledCallback();

	UPROPERTY(EditDefaultsOnly, Category = "Gx|DragonBlade|Slash")
	uint8 CurrentSlashSection;
};
