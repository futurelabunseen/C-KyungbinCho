// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_FireSemiAuto.generated.h"

class AGameplayAbilityTargetActor;
struct FGameplayAbilityTargetDataHandle;

/**
 * UGxGameplayAbility_FireSemiAuto
 *
 * 단발 반자동 발사 어빌리티다.
 * 연사 발사 어빌리티에 의해 계속해서 트리거되는 방식이다.
 */
UCLASS(Meta = (ShortTooltip = "Semi-automatic fire ability. Triggered by automatic fire abilities."))
class UGxGameplayAbility_FireSemiAuto : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_FireSemiAuto(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGxGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGxGameplayAbility interface

	UPROPERTY(EditDefaultsOnly, Category = "Gx|FireSemiAuto")
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

	// [TODO]
	// 데미지도 공격 어빌리티, 비공격 어빌리티로 나누는 게 좋을 것 같다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gx|FireSemiAuto")
	float Damage;

private:
	UFUNCTION()
	void ValidDataCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void CancelledCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
