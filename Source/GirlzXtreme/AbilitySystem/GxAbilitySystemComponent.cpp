// Fill out your copyright notice in the Description page of Project Settings.

#include "GxAbilitySystemComponent.h"

#include "GxLogChannels.h"
#include "AbilitySystem/GxGlobalAbilitySystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAbilitySystemComponent)

//////////////////////////////////////////////////////////////////////////
// UGxAbilitySystemComponent

UGxAbilitySystemComponent::UGxAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGxAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGxGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGxGlobalAbilitySystem>(GetWorld()))
	{
		GlobalAbilitySystem->UnregisterASC(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UGxAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	gxcheck(ActorInfo);
	gxcheck(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		if (UGxGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGxGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}
	}
}
