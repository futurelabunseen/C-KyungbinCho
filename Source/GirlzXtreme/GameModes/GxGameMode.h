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
class AGxGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	AGxGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
