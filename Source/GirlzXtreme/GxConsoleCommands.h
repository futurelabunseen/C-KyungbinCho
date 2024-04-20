// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * GxConsoleCommands
 *
 * 게임 시작시 자동으로 실행할 디버깅용 콘솔 명령어들
 * PlayerController::ConsoleCommand(DEFAULT_CONSLE_COMMANDS) 를 통해 실행
 */

#if !UE_BUILD_SHIPPING
	#define GX_ENABLE_DEFAULT_CONSOLE_COMMANDS 1

	#define GX_DEFAULT_CONSLE_COMMANDS TEXT( \
		"ShowDebug AbilitySystem" " | " \
		"Show Collision" " | " \
		"t.MaxFPS 60" \
	)
#endif // #if !UE_BUILD_SHIPPING
