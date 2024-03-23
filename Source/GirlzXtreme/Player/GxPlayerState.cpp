// Fill out your copyright notice in the Description page of Project Settings.

#include "GxPlayerState.h"

#include "GxLogChannels.h"
#include "Player/GxPlayerController.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "UObject/ConstructorHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxPlayerState)

//////////////////////////////////////////////////////////////////////////
// AGxPlayerState

AGxPlayerState::AGxPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGxAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	/* [TODO]
	CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<ULyraCombatSet>(TEXT("CombatSet"));
	*/ UE_DEBUG_BREAK();
}

AGxPlayerController* AGxPlayerState::GetGxPlayerController() const
{
	return Cast<AGxPlayerController>(GetOwner());
}

UAbilitySystemComponent* AGxPlayerState::GetAbilitySystemComponent() const
{
	return GetGxAbilitySystemComponent();
}

void AGxPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	gxcheck(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}
