// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "Input/GxInputTypes.h"

#include "GxInputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * UGxInputConfig
 *
 * Input Action 목록을 저장한다.
 */
UCLASS(BlueprintType, Const, Meta = (ShortTooltip = "Non-mutable data asset that contains input configuration properties."))
class UGxInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// 입력 매핑들
	// 키는 우선 순위를 저장한다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputMappingContext"))
	TMap<uint8, TObjectPtr<const UInputMappingContext>> InputMappingContexts;

	// 어빌리티가 아닌 일반 Input Action
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TMap<EGxNativeInputType, TObjectPtr<const UInputAction>> NativeInputActions;

	// 입력으로 트리거되는 어빌리티의 Input Action
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TMap<EGxAbilityInputID, TObjectPtr<const UInputAction>> AbilityInputActions;
};
