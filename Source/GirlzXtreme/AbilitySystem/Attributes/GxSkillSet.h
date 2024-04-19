// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/GxAttributeSet.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "NativeGameplayTags.h"

#include "GxSkillSet.generated.h"

struct FGameplayEffectModCallbackData;

/**
 * UGxSkillSet
 *
 * 캐릭터의 스킬 속성들 (궁극기 게이지, ...)
 */
UCLASS(BlueprintType, Meta = (ShortTooltip = "Class that defines attributes that are necessary for using skills. Attribute examples include: ultimate gauge."))
class UGxSkillSet : public UGxAttributeSet
{
	GENERATED_BODY()

public:
	UGxSkillSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATTRIBUTE_ACCESSORS(UGxSkillSet, UltimateGauge);
	ATTRIBUTE_ACCESSORS(UGxSkillSet, MaxUltimateGauge);

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
	// The current ultimate gauge attribute.  The ultimate gauge will be capped by the max ultimate gauge attribute.
	// Ultimate gauge is hidden from modifiers so only executions can modify it.
	// [TODO]
	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UltimateGauge, Category = "Gx|Skill", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UltimateGauge, Category = "Gx|Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData UltimateGauge;

	// The current max ultimate gauge attribute.  Max ultimate gauge is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxUltimateGauge, Category = "Gx|Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxUltimateGauge;

	UFUNCTION()
	void OnRep_UltimateGauge(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxUltimateGauge(const FGameplayAttributeData& OldValue);
};
