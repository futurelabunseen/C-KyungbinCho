// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"

#include "GxAttributeSet.generated.h"

class AActor;
class UGxAbilitySystemComponent;
struct FGameplayEffectSpec;

/**
 * This macro defines a set of helper functions for accessing and initializing attributes.
 *
 * The following example of the macro:
 *		ATTRIBUTE_ACCESSORS(UGxHealthSet, Health)
 * 
 * will create the following functions:
 *		static	FGameplayAttribute GetHealthAttribute();
 *		float	GetHealth() const;
 *		void	SetHealth(float NewVal);
 *		void	InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Delegate used to broadcast attribute events.
DECLARE_MULTICAST_DELEGATE_FourParams(FGxAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 * UGxAttributeSet
 *
 * 게임에서 사용될 기본 Attribute Set
 */
UCLASS(Meta = (ShortTooltip = "Base attribute set class for the project."))
class UGxAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGxAttributeSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const;

	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const { }
};
