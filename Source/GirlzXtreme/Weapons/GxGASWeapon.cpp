// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGASWeapon.h"

#include "GxLogChannels.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGASWeapon)

//////////////////////////////////////////////////////////////////////////
// AGxGASWeapon

AGxGASWeapon::AGxGASWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGxAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AGxGASWeapon::GetAbilitySystemComponent() const
{
	return GetGxAbilitySystemComponent();
}

void AGxGASWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	gxcheck(AbilitySystemComponent);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->GiveAbilities(DefaultAbilities);
	AbilitySystemComponent->ApplyEffects(DefaultEffects);
}
