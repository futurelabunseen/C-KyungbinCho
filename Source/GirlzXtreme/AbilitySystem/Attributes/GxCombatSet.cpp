// Fill out your copyright notice in the Description page of Project Settings.

#include "GxCombatSet.h"

#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// UGxCombatSet

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxCombatSet)

UGxCombatSet::UGxCombatSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UGxCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGxCombatSet, BaseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGxCombatSet, BaseHeal, COND_None, REPNOTIFY_Always);
}

void UGxCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCombatSet, BaseDamage, OldValue);
}

void UGxCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGxCombatSet, BaseHeal, OldValue);
}
