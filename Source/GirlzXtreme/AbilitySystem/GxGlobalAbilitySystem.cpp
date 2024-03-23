// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGlobalAbilitySystem.h"

#include "GxLogChannels.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGlobalAbilitySystem)

//////////////////////////////////////////////////////////////////////////
// FGlobalAppliedAbilityList

void FGlobalAppliedAbilityList::AddToASC(TSubclassOf<UGameplayAbility> Ability, UGxAbilitySystemComponent* ASC)
{
	if (FGameplayAbilitySpecHandle* SpecHandle = Handles.Find(ASC))
	{
		RemoveFromASC(ASC);
	}

	UGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UGameplayAbility>();
	FGameplayAbilitySpec AbilitySpec(AbilityCDO);
	const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

	Handles.Add(ASC, AbilitySpecHandle);
}

void FGlobalAppliedAbilityList::RemoveFromASC(UGxAbilitySystemComponent* ASC)
{
	if (FGameplayAbilitySpecHandle* SpecHandle = Handles.Find(ASC))
	{
		ASC->ClearAbility(*SpecHandle);
		Handles.Remove(ASC);
	}
}

void FGlobalAppliedAbilityList::RemoveFromAll()
{
	for (auto& [Key, Value] : Handles)
	{
		if (Key != nullptr)
		{
			Key->ClearAbility(Value);
		}
	}

	Handles.Empty();
}

//////////////////////////////////////////////////////////////////////////
// FGlobalAppliedEffectList

void FGlobalAppliedEffectList::AddToASC(TSubclassOf<UGameplayEffect> Effect, UGxAbilitySystemComponent* ASC)
{
	if (FActiveGameplayEffectHandle* EffectHandle = Handles.Find(ASC))
	{
		RemoveFromASC(ASC);
	}

	const UGameplayEffect* GameplayEffectCDO = Effect->GetDefaultObject<UGameplayEffect>();
	const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffectCDO, /*Level=*/ 1, ASC->MakeEffectContext());
	Handles.Add(ASC, GameplayEffectHandle);
}

void FGlobalAppliedEffectList::RemoveFromASC(UGxAbilitySystemComponent* ASC)
{
	if (FActiveGameplayEffectHandle* EffectHandle = Handles.Find(ASC))
	{
		ASC->RemoveActiveGameplayEffect(*EffectHandle);
		Handles.Remove(ASC);
	}
}

void FGlobalAppliedEffectList::RemoveFromAll()
{
	for (auto& [Key, Value] : Handles)
	{
		if (Key != nullptr)
		{
			Key->RemoveActiveGameplayEffect(Value);
		}
	}

	Handles.Empty();
}

//////////////////////////////////////////////////////////////////////////
// UGxGlobalAbilitySystem

UGxGlobalAbilitySystem::UGxGlobalAbilitySystem(const FObjectInitializer& ObjectInitializer)
{
}

void UGxGlobalAbilitySystem::ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability)
{
	if ((Ability.Get() != nullptr) && (!AppliedAbilities.Contains(Ability)))
	{
		FGlobalAppliedAbilityList& Entry = AppliedAbilities.Add(Ability);		
		for (UGxAbilitySystemComponent* ASC : RegisteredASCs)
		{
			Entry.AddToASC(Ability, ASC);
		}
	}
}

void UGxGlobalAbilitySystem::ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect)
{
	if ((Effect.Get() != nullptr) && (!AppliedEffects.Contains(Effect)))
	{
		FGlobalAppliedEffectList& Entry = AppliedEffects.Add(Effect);
		for (UGxAbilitySystemComponent* ASC : RegisteredASCs)
		{
			Entry.AddToASC(Effect, ASC);
		}
	}
}

void UGxGlobalAbilitySystem::RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability)
{
	if ((Ability.Get() != nullptr) && AppliedAbilities.Contains(Ability))
	{
		FGlobalAppliedAbilityList& Entry = AppliedAbilities[Ability];
		Entry.RemoveFromAll();
		AppliedAbilities.Remove(Ability);
	}
}

void UGxGlobalAbilitySystem::RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect)
{
	if ((Effect.Get() != nullptr) && AppliedEffects.Contains(Effect))
	{
		FGlobalAppliedEffectList& Entry = AppliedEffects[Effect];
		Entry.RemoveFromAll();
		AppliedEffects.Remove(Effect);
	}
}

void UGxGlobalAbilitySystem::RegisterASC(UGxAbilitySystemComponent* ASC)
{
	gxcheck(ASC);

	for (auto& [Key, Value] : AppliedAbilities)
	{
		Value.AddToASC(Key, ASC);
	}
	for (auto& [Key, Value] : AppliedEffects)
	{
		Value.AddToASC(Key, ASC);
	}

	RegisteredASCs.AddUnique(ASC);
}

void UGxGlobalAbilitySystem::UnregisterASC(UGxAbilitySystemComponent* ASC)
{
	gxcheck(ASC);

	for (auto& [Key, Value] : AppliedAbilities)
	{
		Value.RemoveFromASC(ASC);
	}
	for (auto& [Key, Value] : AppliedEffects)
	{
		Value.RemoveFromASC(ASC);
	}

	RegisteredASCs.Remove(ASC);
}
