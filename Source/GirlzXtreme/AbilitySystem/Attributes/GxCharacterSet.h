// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/GxAttributeSet.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"

#include "GxCharacterSet.generated.h"

struct FGameplayEffectModCallbackData;

/**
 * UGxCharacterSet
 *
 * 캐릭터의 속성들 (걷기 속도, 앉기 속도, 공격 속도 비율, ...)
 * 1.0f 는 기본 속도를 의미한다.
 */
UCLASS(BlueprintType, Meta = (ShortTooltip = "Class that defines attributes that are necessary for character. Attribute examples include: walk speed, crouched speed, and attack speed rate."))
class UGxCharacterSet : public UGxAttributeSet
{
	GENERATED_BODY()

public:
	UGxCharacterSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATTRIBUTE_ACCESSORS(UGxCharacterSet, WalkSpeed);
	ATTRIBUTE_ACCESSORS(UGxCharacterSet, MaxWalkSpeed);
	ATTRIBUTE_ACCESSORS(UGxCharacterSet, CrouchedSpeed);
	ATTRIBUTE_ACCESSORS(UGxCharacterSet, MaxCrouchedSpeed);
	ATTRIBUTE_ACCESSORS(UGxCharacterSet, AttackSpeedRate);
	ATTRIBUTE_ACCESSORS(UGxCharacterSet, MaxAttackSpeedRate);

	//~UObject interface
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

protected:
	//~UAttributeSet interface
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~End of UAttributeSet interface

	//~UGxAttributeSet interface
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//~End of UGxAttributeSet interface

private:
	// The current walk speed attribute.  The walk speed will be capped by the max walk speed attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WalkSpeed, Category = "Gx|Character", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WalkSpeed;

	// The current max walk speed attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxWalkSpeed, Category = "Gx|Character", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxWalkSpeed;

	// The current crouched speed attribute.  The crouched speed will be capped by the max crouched speed attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CrouchedSpeed, Category = "Gx|Character", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CrouchedSpeed;

	// The current max crouched speed attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCrouchedSpeed, Category = "Gx|Character", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxCrouchedSpeed;

	// The current attack speed rate attribute.  The attack speed rate will be capped by the max attack speed rate attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeedRate, Category = "Gx|Character", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeedRate;

	// The current max attack speed rate attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAttackSpeedRate, Category = "Gx|Character", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackSpeedRate;

	UFUNCTION()
	void OnRep_WalkSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxWalkSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CrouchedSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxCrouchedSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AttackSpeedRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxAttackSpeedRate(const FGameplayAttributeData& OldValue);
};
