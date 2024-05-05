// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameMode.h"

#include "GxLogChannels.h"
#include "GameFramework/GameState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameMode)

//////////////////////////////////////////////////////////////////////////
// AGxGameMode

AGxGameMode::AGxGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MaxPlayers(4)
{
	GameStateClass = AGameState::StaticClass();

	bUseSeamlessTravel = true;
}

void AGxGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (NumPlayers < MaxPlayers)
	{
		Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	}
	else
	{
		ErrorMessage = TEXT("[FAIL] Session already full.");
		FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
	}
}

void AGxGameMode::PostLogin(APlayerController* NewPlayer)
{
	GX_NET_LOG(Warning, TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	gxcheck(NetDriver);

	if (NetDriver->ClientConnections.Num() == 0)
	{
		GX_NET_LOG(Warning, TEXT("No Client Connection"));
	}

	for (const auto& Connection : NetDriver->ClientConnections)
	{
		GX_NET_LOG(Warning, TEXT("Client Connections: %s"), *Connection->GetName());
	}

	GX_NET_LOG(Warning, TEXT("End"));
}
