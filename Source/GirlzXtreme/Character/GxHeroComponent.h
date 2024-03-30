// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PawnComponent.h"
#include "Input/GxInputTypes.h"

#include "GxHeroComponent.generated.h"

class UInputComponent;
struct FInputActionValue;

/**
 * UGxHeroComponent
 * 
 * 입력 처리를 캐릭터에서 분리해 뺀 컴포넌트
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent, ShortTooltip = "Component that sets up input handling for player controlled pawns."))
class UGxHeroComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UGxHeroComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

protected:
	void Input_AbilityInputPressed(EGxAbilityInputID InputID);
	void Input_AbilityInputReleased(EGxAbilityInputID InputID);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
};
