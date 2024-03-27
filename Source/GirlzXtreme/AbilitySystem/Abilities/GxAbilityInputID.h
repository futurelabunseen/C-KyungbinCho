// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Types/AttributeStorage.h"

/**
 * EGxAbilityInputID
 *
 * 각 GA 의 Input ID 를 선언한 파일
 * 
 * 리플리케이션에 활용될 수 있는 열거형은 enum class 가 아닌 enum 으로 선언해 TEnumAsByte<E> 형식으로 써야 한다.
 * 이 입력 ID 들은 리플리케이션이 필요 없기 때문에 enum class 로 선언
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
	// 3	Attack
	Attack		UMETA(DisplayName = "Attack"),
	// 4	Skill1
	Skill1		UMETA(DisplayName = "Skill1"),
	// 5	Skill2
	Skill2		UMETA(DisplayName = "Skill2"),
	// 6	Ultimate
	Ultimate	UMETA(DisplayName = "Ultimate"),

	// Num of enumerators
	MAX			UMETA(Hidden)
};

template <typename E>
constexpr auto ToUtype(E enumerator) noexcept
{
	return StaticCast<UE::Interchange::Private::TUnderlyingType<E>::type>(enumerator);
}
