// Fill out your copyright notice in the Description page of Project Settings.

#include "GxLobbyGameMode.h"

#include "GxLogChannels.h"
#include "GameModes/GxGameState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxLobbyGameMode)

//////////////////////////////////////////////////////////////////////////
// AGxLobbyGameMode

AGxLobbyGameMode::AGxLobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AGxLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumCurrentPlayers = GameState->PlayerArray.Num();

	if (MaxPlayers <= NumCurrentPlayers)
	{
		UWorld* World = GetWorld();
		gxcheck(World);

		World->ServerTravel(ServerTravelPath);
	}
}
