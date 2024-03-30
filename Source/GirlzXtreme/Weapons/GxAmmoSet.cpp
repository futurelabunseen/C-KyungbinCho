// Fill out your copyright notice in the Description page of Project Settings.

#include "GxAmmoSet.h"

#include "GxLogChannels.h"
#include "GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAmmoSet)

//////////////////////////////////////////////////////////////////////////
// UGxAmmoSet

UGxAmmoSet::UGxAmmoSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Ammo(0.0f)
	, MaxAmmo(30.0f)
{
}

bool UGxAmmoSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UGxAmmoSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetAmmoAttribute())
	{
		// Clamp ammo
		float NewValue = GetAmmo();
		ClampAttribute(GetAmmoAttribute(), NewValue);

		SetAmmo(NewValue);
	}
}

void UGxAmmoSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGxAmmoSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UGxAmmoSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxASC);

	if (Attribute == GetMaxAmmoAttribute())
	{
		// Make sure current movement speed rate is not greater than the new max movement speed rate.
		if (GetAmmo() > NewValue)
		{
			GxASC->ApplyModToAttribute(GetAmmoAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void UGxAmmoSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);

	if (Attribute == GetAmmoAttribute())
	{
		// Do not allow ammo to go negative or above max ammo.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAmmo());
	}
	else if (Attribute == GetMaxAmmoAttribute())
	{
		// Do not allow max ammo to drop below 0.
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
