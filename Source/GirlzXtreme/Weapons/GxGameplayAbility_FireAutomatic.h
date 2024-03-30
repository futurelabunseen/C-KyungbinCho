// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/GxGameplayAbility.h"

#include "GxGameplayAbility_FireAutomatic.generated.h"

class AGxGASGun;
class UGxAbilitySystemComponent;

/**
 * UGxGameplayAbility_FireAutomatic
 *
 * 연사 자동 소총 발사 어빌리티다.
 * Repeat Ability Task 를 통해 매 TimeBetweenShots 마다 반자동 발사 어빌리티를 트리거하는 방식으로 동작한다.
 */
UCLASS(Meta = (ShortTooltip = "Automatic fire ability. With repeat ability task, this trigger semi auto ability every TimeBetweenShots."))
class UGxGameplayAbility_FireAutomatic : public UGxGameplayAbility
{
	GENERATED_BODY()

public:
	UGxGameplayAbility_FireAutomatic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility interface
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UGameplayAbility interface

	//~UGxGameplayAbility interface
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGxGameplayAbility interface

	// 연사 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gx|FireAutomatic")
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Gx|FireAutomatic")
	TSubclassOf<UGxGameplayAbility> FireSemiAutoAbility;

	// [TODO] 임시 코드
	UPROPERTY(EditDefaultsOnly, Category = "Gx|FireAutomatic")
	TSubclassOf<UGameplayEffect> FireCostEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Gx|FireAutomatic")
	TSubclassOf<UGameplayEffect> ReloadEffect;

	AGxGASGun* GetGxGASGun();
	UGxAbilitySystemComponent* GetGxGunASC();

private:
	UFUNCTION()
	void OnPerformActionCallback(int32 ActionNumber);

	UFUNCTION()
	void OnFinishedCallback(int32 ActionNumber);

	void ApplyGameplayEffectToGun(const TSubclassOf<UGameplayEffect>& EffectClass);
};
