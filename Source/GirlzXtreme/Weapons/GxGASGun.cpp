// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGASGun.h"

#include "GxLogChannels.h"
#include "Weapons/GxAmmoSet.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGASGun)

//////////////////////////////////////////////////////////////////////////
// AGxGASGun

AGxGASGun::AGxGASGun(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AmmoSet = ObjectInitializer.CreateDefaultSubobject<UGxAmmoSet>(this, TEXT("AmmoSet"));
}

void AGxGASGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	gxcheck(AbilitySystemComponent);
	gxcheck(AmmoSet);

	// Subscribe to UGxAmmoSet::Ammo change
	OnAmmoAttributeChangeHandle = AbilitySystemComponent->AddOnAttributeChange(AmmoSet->GetAmmoAttribute(), this, &ThisClass::OnAttributeChangedCallback);
}

void AGxGASGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	gxcheck(AbilitySystemComponent);
	gxcheck(AmmoSet);

	// Unsubscribe to UGxAmmoSet::Ammo change
	AbilitySystemComponent->RemoveOnAttributeChange(AmmoSet->GetAmmoAttribute(), OnAmmoAttributeChangeHandle);

	Super::EndPlay(EndPlayReason);
}

void AGxGASGun::OnAttributeChangedCallback(const FOnAttributeChangeData& AttributeChangedData) const
{
	FGameplayAttribute ChangedAttribute = AttributeChangedData.Attribute;
	float NewValue = AttributeChangedData.NewValue;

	if (ChangedAttribute == AmmoSet->GetAmmoAttribute())
	{
		// [TODO]
		UE_DEBUG_BREAK();
	}
}

int32 AGxGASGun::GetAmmo()
{
	return StaticCast<int32>(AmmoSet ? AmmoSet->GetAmmo() : -1);
}

int32 AGxGASGun::GetMaxAmmo()
{
	return StaticCast<int32>(AmmoSet ? AmmoSet->GetMaxAmmo() : -1);
}
