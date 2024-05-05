// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularGameMode.h"

#include "GxGameMode.generated.h"

/**
 * AGxGameMode
 *
 * 게임에서 사용되는 게임 모드 Base
 * 모듈형 게임플레이와 함께 사용될 수 있다.
 */
UCLASS(Meta = (ShortTooltip = "The base game mode class used by this project."))
class AGxGameMode : public AModularGameMode
{
	GENERATED_BODY()

public:
	AGxGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|GameMode")
	int32 MaxPlayers;
};
