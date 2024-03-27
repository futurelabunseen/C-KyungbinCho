// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularGameState.h"
#include "AbilitySystemInterface.h"

#include "GxGameState.generated.h"

class UAbilitySystemComponent;
class UGxAbilitySystemComponent;

/**
 * AGxGameState
 *
 * 게임에서 사용되는 게임 스테이트 Base
 * 모듈형 게임플레이와 함께 사용될 수 있다.
 */
UCLASS(Meta = (ShortTooltip = "The base game state class used by this project."))
class AGxGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGxGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameStateBase interface
	virtual void PostInitializeComponents() override;
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//~End of AGameStateBase interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	UFUNCTION(BlueprintCallable, Category = "Gx|GameState")
	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const { return AbilitySystemComponent; }

private:
	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "Gx|GameState")
	TObjectPtr<UGxAbilitySystemComponent> AbilitySystemComponent;
};
