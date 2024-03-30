// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GxUtilityFunctions.h"

/**
 * EGxNativeInputType
 *
 * 각 Input Action 에 1:1 매핑되는 열거형
 * 
 * 리플리케이션에 사용될 수 있는 열거형은 enum class 가 아닌 enum 으로 선언해 TEnumAsByte<E> 형식으로 써야 한다.
 * 이 열거형은 리플리케이션에 쓰이지 않기 때문에 enum class 로 정의
 */
UENUM(BlueprintType)
enum class EGxNativeInputType : uint8
{
	// 0	None
	None		UMETA(DisplayName = "None"),

	// 1	Move
	Move		UMETA(DisplayName = "Move"),
	// 2	Look
	Look		UMETA(DisplayName = "Look"),

	// Num of enumerators
	MAX			UMETA(Hidden)
};

/**
 * EGxAbilityInputID
 *
 * 각 GA 의 Input ID
 * 
 * EGxNativeInputType 과 같은 이유로 enum class 로 정의
 */
UENUM(BlueprintType)
enum class EGxAbilityInputID : uint8
{
	// 0	None
	None		UMETA(DisplayName = "None"),

	// 1	Jump
	Jump		UMETA(DisplayName = "Jump"),
	// 2	Crouch
	Crouch		UMETA(DisplayName = "Crouch"),
	// 3	Reload
	Reload		UMETA(DisplayName = "Reload"),
	// 4	Attack
	Attack		UMETA(DisplayName = "Attack"),
	// 5	Skill1
	Skill1		UMETA(DisplayName = "Skill1"),
	// 6	Skill2
	Skill2		UMETA(DisplayName = "Skill2"),
	// 7	Ultimate
	Ultimate	UMETA(DisplayName = "Ultimate"),

	// Num of enumerators
	MAX			UMETA(Hidden)
};
