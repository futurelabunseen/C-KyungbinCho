// Fill out your copyright notice in the Description page of Project Settings.

#include "GxSkillSet.h"

#include "GxLogChannels.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxSkillSet)

//////////////////////////////////////////////////////////////////////////
// UGxSkillSet

UGxSkillSet::UGxSkillSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, UltimateGauge(0.0f)
	, MaxUltimateGauge(900.0f)
{
}

void UGxSkillSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGxSkillSet, UltimateGauge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxSkillSet, MaxUltimateGauge, COND_None, REPNOTIFY_Always);
}

void UGxSkillSet::OnRep_UltimateGauge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxSkillSet, UltimateGauge, OldValue);
}

void UGxSkillSet::OnRep_MaxUltimateGauge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxSkillSet, MaxUltimateGauge, OldValue);
}

bool UGxSkillSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UGxSkillSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetUltimateGaugeAttribute())
	{
		// Clamp ultimate gauge
		float NewValue = GetUltimateGauge();
		ClampAttribute(GetUltimateGaugeAttribute(), NewValue);

		SetUltimateGauge(NewValue);
	}
}

void UGxSkillSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGxSkillSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGxSkillSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxASC);

	if (Attribute == GetMaxUltimateGaugeAttribute())
	{
		// Make sure current movement speed rate is not greater than the new max movement speed rate.
		if (GetUltimateGauge() > NewValue)
		{
			GxASC->ApplyModToAttribute(GetUltimateGaugeAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void UGxSkillSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);

	if (Attribute == GetUltimateGaugeAttribute())
	{
		// Do not allow ultimate gauge to go negative or above max ultimate gauge.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxUltimateGauge());
	}
	else if (Attribute == GetMaxUltimateGaugeAttribute())
	{
		// Do not allow max ultimate gauge to drop below 0.
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
