// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonPlayerController.h"

#include "GxPlayerController.generated.h"

class APawn;
class AGxPlayerState;
class UGxAbilitySystemComponent;
class UGxInGameUI;

/**
 * AGxPlayerController
 *
 * 게임에서 사용되는 플레이어 컨트롤러
 * 모듈형 게임플레이와 함께 사용될 수 있다.
 */
UCLASS(Meta = (ShortTooltip = "The base player controller class used by this project."))
class AGxPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	AGxPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Gx|PlayerController")
	AGxPlayerState* GetGxPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Gx|PlayerController")
	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const;

	//~ACommonPlayerController interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	//~End of ACommonPlayerController interface

public:
	UFUNCTION(BlueprintCallable, Category = "Gx|UI")
	UGxInGameUI* GetGxInGameUI() const { return GxInGameUI; };

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Gx|UI")
    TSubclassOf<UGxInGameUI> GxInGameUIClass;

    UPROPERTY(VisibleInstanceOnly, Category = "Gx|UI")
    UGxInGameUI* GxInGameUI;
};
