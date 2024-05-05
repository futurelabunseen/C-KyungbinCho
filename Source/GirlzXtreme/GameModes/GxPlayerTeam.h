// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * EGxPlayerTeam
 *
 * 점령전에서의 각 팀을 정의
 */
UENUM(BlueprintType)
enum class EGxPlayerTeam : uint8
{
	// 0	None
	None		UMETA(DisplayName = "None"),

	// 1	TeamA
	TeamA		UMETA(DisplayName = "TeamA"),
	// 2	TeamB
	TeamB		UMETA(DisplayName = "TeamB"),

	// Num of enumerators
	MAX			UMETA(Hidden)
};
