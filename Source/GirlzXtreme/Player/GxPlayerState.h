// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"

#include "GxPlayerState.generated.h"

class AGxPlayerController;
class UAbilitySystemComponent;
class UGxAbilitySystemComponent;

/**
 * AGxPlayerState
 * 
 * 게임에서 사용되는 플레이어 스테이트
 * 모듈형 게임플레이와 함께 사용될 수 있다.
 */
UCLASS(Meta = (ShortTooltip = "The base player state class used by this project."))
class AGxPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGxPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~APlayerState interface
	virtual void PostInitializeComponents() override;
	//~End of APlayerState interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	UFUNCTION(BlueprintCallable, Category = "Gx|PlayerState")
	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable, Category = "Gx|PlayerState")
	AGxPlayerController* GetGxPlayerController() const;

private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Gx|PlayerState")
	TObjectPtr<UGxAbilitySystemComponent> AbilitySystemComponent;
};
