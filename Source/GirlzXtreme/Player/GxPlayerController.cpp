// Fill out your copyright notice in the Description page of Project Settings.

#include "GxPlayerController.h"

#include "GxLogChannels.h"
#include "GxConsoleCommands.h"
#include "Player/GxPlayerState.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "UI/GxInGameUI.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxPlayerController)

//////////////////////////////////////////////////////////////////////////
// AGxPlayerController

AGxPlayerController::AGxPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

AGxPlayerState* AGxPlayerController::GetGxPlayerState() const
{
	return CastChecked<AGxPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGxAbilitySystemComponent* AGxPlayerController::GetGxAbilitySystemComponent() const
{
	const AGxPlayerState* GxPS = GetGxPlayerState();
	return (GxPS ? GxPS->GetGxAbilitySystemComponent() : nullptr);
}

void AGxPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GxInGameUI = CreateWidget<UGxInGameUI>(this, GxInGameUIClass);
	gxcheck(GxInGameUI);

	GxInGameUI->AddToViewport();
}

void AGxPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if GX_ENABLE_DEFAULT_CONSOLE_COMMANDS
	// Execute default console commands for debugging.
	ConsoleCommand(GX_DEFAULT_CONSLE_COMMANDS);
#endif // #if GX_ENABLE_DEFAULT_CONSOLE_COMMANDS
}

void AGxPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}
