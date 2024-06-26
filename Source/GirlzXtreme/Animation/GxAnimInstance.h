// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"

#include "GxAnimInstance.generated.h"

/**
 * UGxAnimInstance
 *
 * 게임에서 기본으로 사용되는 애님 인스턴스
 */
UCLASS(Meta = (ShortTooltip = "The base game animation instance class used by this project."))
class UGxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGxAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetIsPlayingAbilityMonatage(bool bNewIsPlaying) { bIsPlayingAbilityMontage = bNewIsPlaying; }
	bool GetIsPlayingAbilityMonatage() const { return bIsPlayingAbilityMontage; }

	void IncreaseAbilityAnimStack() { ++NumAbilityAnimStack; }
	void DecreaseAbilityAnimStack() { --NumAbilityAnimStack; }

protected:
	//~UAnimInstance interface
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~End of UAnimInstance interface

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|AnimInstance")
    uint8 bIsPlayingAbilityMontage:1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gx|AnimInstance")
    uint8 NumAbilityAnimStack;
};
