// Fill out your copyright notice in the Description page of Project Settings.

#include "GxAttributeSet.h"

#include "AbilitySystem/GxAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAttributeSet)

//////////////////////////////////////////////////////////////////////////
// UGxAttributeSet

UGxAttributeSet::UGxAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGxAbilitySystemComponent* UGxAttributeSet::GetGxAbilitySystemComponent() const
{
	return Cast<UGxAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
