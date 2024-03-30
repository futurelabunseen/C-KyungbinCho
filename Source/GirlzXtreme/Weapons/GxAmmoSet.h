// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/GxAttributeSet.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "NativeGameplayTags.h"

#include "GxAmmoSet.generated.h"

struct FGameplayEffectModCallbackData;

/**
 * UGxAmmoSet
 *
 * 무기의 탄창 속성
 */
UCLASS(BlueprintType, Meta = (ShortTooltip = "Class that defines attributes that are necessary for implementing ammo. Attribute examples include: ammo."))
class UGxAmmoSet : public UGxAttributeSet
{
	GENERATED_BODY()

public:
	UGxAmmoSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATTRIBUTE_ACCESSORS(UGxAmmoSet, Ammo);
	ATTRIBUTE_ACCESSORS(UGxAmmoSet, MaxAmmo);

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
	// The current ammo attribute.  The ammo will be capped by the max ammo attribute.
	UPROPERTY(BlueprintReadOnly, Category = "Gx|Ammo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ammo;

	// The current max ammo attribute.  Max ammo is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "Gx|Ammo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAmmo;
};
