// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameModes/GxGameMode.h"

#include "GxLobbyGameMode.generated.h"

/**
 * AGxLobbyGameMode
 *
 * 게임에서 사용되는 Lobby 맵 게임 모드
 */
UCLASS(Meta = (ShortTooltip = "The Lobby game mode class used by this project."))
class AGxLobbyGameMode : public AGxGameMode
{
	GENERATED_BODY()

public:
	AGxLobbyGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|GameMode|Lobby")
	int32 MaxPlayers;

	UPROPERTY(EditDefaultsOnly, Category = "Gx|GameMode|Lobby")
	FString ServerTravelPath;
};
