// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/GxAttributeSet.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "NativeGameplayTags.h"

#include "GxHealthSet.generated.h"

// 데미지 면역 태그
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);

struct FGameplayEffectModCallbackData;

/**
 * UGxHealthSet
 *
 * 체력과 메타 속성(데미지, 힐량, ...)들
 */
UCLASS(BlueprintType, Meta = (ShortTooltip = "Class that defines attributes that are necessary for taking damage. Attribute examples include: health, shields, and resistances."))
class UGxHealthSet : public UGxAttributeSet
{
	GENERATED_BODY()

public:
	UGxHealthSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATTRIBUTE_ACCESSORS(UGxHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UGxHealthSet, MaxHealth);
	// Meta Attributes
	ATTRIBUTE_ACCESSORS(UGxHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UGxHealthSet, Damage);

	// Delegate to broadcast when the health attribute reaches zero.
	mutable FGxAttributeEvent OnOutOfHealth;

	//~UObject interface
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

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
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	// [TODO]
	//UPROPERTY(BlueprintReadOnly, Category = "Gx|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Gx|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Gx|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------

private:
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category = "Gx|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	// Damage is hidden from modifiers so only executions can modify it.
	// [TODO]
	//UPROPERTY(BlueprintReadOnly, Category="Gx|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	UPROPERTY(BlueprintReadOnly, Category = "Gx|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};
