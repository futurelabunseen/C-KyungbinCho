// Fill out your copyright notice in the Description page of Project Settings.

#include "GxPlayerController.h"

#include "GxConsoleCommands.h"
#include "GxLogChannels.h"
#include "Player/GxPlayerState.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputSubsystems.h"

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

void AGxPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if !UE_BUILD_SHIPPING
	// Execute default console commands for debugging.
	ConsoleCommand(DEFAULT_CONSLE_COMMANDS);
#endif // #if !UE_BUILD_SHIPPING
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

void AGxPlayerController::ClearAllMappings()
{
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	gxcheck(EILPSubsystem);

	EILPSubsystem->ClearAllMappings();
}

void AGxPlayerController::AddMappingContext(UInputMappingContext* NewMappingContext, int32 Priority)
{
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	gxcheck(EILPSubsystem);
	gxcheck(NewMappingContext);

	EILPSubsystem->AddMappingContext(NewMappingContext, Priority);
}

void AGxPlayerController::RemoveMappingContext(UInputMappingContext* MappingContext)
{
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	gxcheck(EILPSubsystem);
	gxcheck(MappingContext);

	EILPSubsystem->RemoveMappingContext(MappingContext);
}
