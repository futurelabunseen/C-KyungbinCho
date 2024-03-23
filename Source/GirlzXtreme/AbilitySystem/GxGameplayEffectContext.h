// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"

#include "GxGameplayEffectContext.generated.h"

class AActor;
class IGxAbilitySourceInterface;
class UObject;
class UPhysicalMaterial;

/**
 * FGxGameplayEffectContext
 *
 * 게임에서 사용될 기본 Gameplay Effect Context
 */
USTRUCT()
struct FGxGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FGxGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FGxGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	/** Returns the wrapped FLyraGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static FGxGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const IGxAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const IGxAbilitySourceInterface* GetAbilitySource() const;

	//~FGameplayEffectContext interface
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FGxGameplayEffectContext* NewContext = new FGxGameplayEffectContext();
		*NewContext = *this;

		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}

		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGxGameplayEffectContext::StaticStruct();
	}
	//~End of FGameplayEffectContext interface

	/** Returns the physical material from the hit result if there is one */
	const UPhysicalMaterial* GetPhysicalMaterial() const;

protected:
	/** Ability Source object (should implement IGxAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};
