// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayEffectContext.h"

#include "AbilitySystem/GxAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayEffectContext)

//////////////////////////////////////////////////////////////////////////
// FGxGameplayEffectContext

FGxGameplayEffectContext* FGxGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();

	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FGxGameplayEffectContext::StaticStruct()))
	{
		return (FGxGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FGxGameplayEffectContext::SetAbilitySource(const IGxAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IGxAbilitySourceInterface* FGxGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IGxAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FGxGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}

	return nullptr;
}
