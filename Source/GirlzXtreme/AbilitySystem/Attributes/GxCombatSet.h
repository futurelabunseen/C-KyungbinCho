// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/GxAttributeSet.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"

#include "GxCombatSet.generated.h"

/**
 * UGxCombatSet
 *
 * 전투에서 계산에 사용되는 속성의 기본 값들 (기본 데미지, 기본 힐량, ...)
 */
UCLASS(BlueprintType, Meta = (ShortTooltip = "Class that defines attributes that are necessary for applying damage or healing. Attribute examples include: damage, healing, attack power, and shield penetrations."))
class UGxCombatSet : public UGxAttributeSet
{
	GENERATED_BODY()

public:
	UGxCombatSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATTRIBUTE_ACCESSORS(UGxCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UGxCombatSet, BaseHeal);

	//~UObject interface
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

private:
	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Gx|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Gx|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);
};
