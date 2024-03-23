// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameState.h"

#include "GxLogChannels.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameState)

//////////////////////////////////////////////////////////////////////////
// AGxGameState

AGxGameState::AGxGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGxAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void AGxGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	gxcheck(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

UAbilitySystemComponent* AGxGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGxGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	// Remove inactive and bots
	for (int32 i = PlayerArray.Num() - 1; 0 <= i; --i)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}
