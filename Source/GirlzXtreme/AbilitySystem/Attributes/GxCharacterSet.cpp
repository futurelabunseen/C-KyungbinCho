// Fill out your copyright notice in the Description page of Project Settings.

#include "GxCharacterSet.h"

#include "GxLogChannels.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxCharacterSet)

//////////////////////////////////////////////////////////////////////////
// UGxCharacterSet

UGxCharacterSet::UGxCharacterSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, WalkSpeed(500.0f)
	, MaxWalkSpeed(1000.0f)
	, CrouchedSpeed(300.0f)
	, MaxCrouchedSpeed(600.0f)
	, AttackSpeedRate(1.0f)
	, MaxAttackSpeedRate(2.0f)
{
}

void UGxCharacterSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGxCharacterSet, WalkSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxCharacterSet, MaxWalkSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxCharacterSet, CrouchedSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxCharacterSet, MaxCrouchedSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxCharacterSet, AttackSpeedRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxCharacterSet, MaxAttackSpeedRate, COND_None, REPNOTIFY_Always);
}

void UGxCharacterSet::OnRep_WalkSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCharacterSet, WalkSpeed, OldValue);
}

void UGxCharacterSet::OnRep_MaxWalkSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCharacterSet, MaxWalkSpeed, OldValue);
}

void UGxCharacterSet::OnRep_CrouchedSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCharacterSet, CrouchedSpeed, OldValue);
}

void UGxCharacterSet::OnRep_MaxCrouchedSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCharacterSet, MaxCrouchedSpeed, OldValue);
}

void UGxCharacterSet::OnRep_AttackSpeedRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCharacterSet, AttackSpeedRate, OldValue);
}

void UGxCharacterSet::OnRep_MaxAttackSpeedRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCharacterSet, MaxAttackSpeedRate, OldValue);
}

bool UGxCharacterSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UGxCharacterSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxASC);

	if (Data.EvaluatedData.Attribute == GetWalkSpeedAttribute())
	{
		// Clamp walk speed
		float NewValue = GetWalkSpeed();
		ClampAttribute(GetWalkSpeedAttribute(), NewValue);

		SetWalkSpeed(NewValue);
	}
	else if (Data.EvaluatedData.Attribute == GetCrouchedSpeedAttribute())
	{
		// Clamp crouched speed
		float NewValue = GetCrouchedSpeed();
		ClampAttribute(GetCrouchedSpeedAttribute(), NewValue);

		SetCrouchedSpeed(NewValue);
	}
	else if (Data.EvaluatedData.Attribute == GetAttackSpeedRateAttribute())
	{
		// Clamp attack speed rate
		float NewValue = GetAttackSpeedRate();
		ClampAttribute(GetAttackSpeedRateAttribute(), NewValue);

		SetAttackSpeedRate(NewValue);
	}
}

void UGxCharacterSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGxCharacterSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGxCharacterSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxASC);

	if (Attribute == GetMaxWalkSpeedAttribute())
	{
		// Make sure current walk speed is not greater than the new max walk speed.
		if (GetWalkSpeed() > NewValue)
		{
			GxASC->ApplyModToAttribute(GetWalkSpeedAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
	else if (Attribute == GetMaxCrouchedSpeedAttribute())
	{
		// Make sure current crouched speed is not greater than the new max crouched speed.
		if (GetCrouchedSpeed() > NewValue)
		{
			GxASC->ApplyModToAttribute(GetCrouchedSpeedAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
	else if (Attribute == GetMaxAttackSpeedRateAttribute())
	{
		// Make sure current attack speed rate is not greater than the new max attack speed rate.
		if (GetAttackSpeedRate() > NewValue)
		{
			GxASC->ApplyModToAttribute(GetAttackSpeedRateAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void UGxCharacterSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);

	if (Attribute == GetWalkSpeedAttribute())
	{
		// Do not allow walk speed to drop below 10 or above max walk speed.
		NewValue = FMath::Clamp(NewValue, 10.0f, GetMaxWalkSpeed());
	}
	else if (Attribute == GetCrouchedSpeedAttribute())
	{
		// Do not allow crouched speed to drop below 5 or above max crouched speed.
		NewValue = FMath::Clamp(NewValue, 5.0f, GetMaxCrouchedSpeed());
	}
	else if (Attribute == GetAttackSpeedRateAttribute())
	{
		// Do not allow attack speed rate to go negative or above max attack speed rate.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAttackSpeedRate());
	}
	else if (Attribute == GetMaxWalkSpeedAttribute())
	{
		// Do not allow max walk speed to drop below 30.
		NewValue = FMath::Max(NewValue, 30.0f);
	}
	else if (Attribute == GetMaxCrouchedSpeedAttribute())
	{
		// Do not allow max crouched speed to drop below 20.
		NewValue = FMath::Max(NewValue, 20.0f);
	}
	else if (Attribute == GetMaxAttackSpeedRateAttribute())
	{
		// Do not allow max attack speed rate to drop below 0.1.
		NewValue = FMath::Max(NewValue, 0.1f);
	}
}
