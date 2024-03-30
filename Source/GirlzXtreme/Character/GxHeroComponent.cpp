// Fill out your copyright notice in the Description page of Project Settings.

#include "GxHeroComponent.h"

#include "GxLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Input/GxInputComponent.h"
#include "Character/GxCharacter.h"
#include "Player/GxPlayerController.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxHeroComponent)

//////////////////////////////////////////////////////////////////////////
// UGxHeroComponent

UGxHeroComponent::UGxHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGxHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	gxcheck(PlayerInputComponent);

	const AGxCharacter* GxCharacter = GetPawn<AGxCharacter>();
	gxcheck(GxCharacter);
	const AGxPlayerController* GxPC = GxCharacter->GetGxPlayerController();
	gxcheck(GxPC);
	const ULocalPlayer* LP = GxPC->GetLocalPlayer();
	gxcheck(LP);

	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	gxcheck(EILPSubsystem);

	const UGxInputConfig* InputConfig = GxCharacter->InputConfig;
	gxcheck(InputConfig);

	EILPSubsystem->ClearAllMappings();
	for (const auto& [Priority, InputMappingContext] : InputConfig->InputMappingContexts)
	{
		EILPSubsystem->AddMappingContext(InputMappingContext, Priority);
	}

	UGxInputComponent* GxIC = Cast<UGxInputComponent>(PlayerInputComponent);
	gxcheck(GxIC);

	GxIC->BindNativeAction(InputConfig, EGxNativeInputType::Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	GxIC->BindNativeAction(InputConfig, EGxNativeInputType::Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	GxIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void UGxHeroComponent::Input_AbilityInputPressed(EGxAbilityInputID InputID)
{
	AGxCharacter* GxCharacter = GetPawn<AGxCharacter>();
	gxcheck(GxCharacter);
	UGxAbilitySystemComponent* GxASC = GxCharacter->GetGxAbilitySystemComponent();
	gxcheck(GxASC);
	FGameplayAbilitySpec* GASpec = GxASC->FindAbilitySpecFromInputID(ToUtype(InputID));
	gxcheck(GASpec);

	GASpec->InputPressed = true;
	if (GASpec->IsActive())
	{
		GxASC->AbilitySpecInputPressed(*GASpec);
	}
	else
	{
		GxASC->TryActivateAbility(GASpec->Handle);
	}
}

void UGxHeroComponent::Input_AbilityInputReleased(EGxAbilityInputID InputID)
{
	AGxCharacter* GxCharacter = GetPawn<AGxCharacter>();
	gxcheck(GxCharacter);
	UGxAbilitySystemComponent* GxASC = GxCharacter->GetGxAbilitySystemComponent();
	gxcheck(GxASC);
	FGameplayAbilitySpec* GASpec = GxASC->FindAbilitySpecFromInputID(ToUtype(InputID));
	gxcheck(GASpec);

	GASpec->InputPressed = false;
	if (GASpec->IsActive())
	{
		GxASC->AbilitySpecInputReleased(*GASpec);
	}
}

void UGxHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	AGxCharacter* GxCharacter = GetPawn<AGxCharacter>();
	gxcheck(GxCharacter);
	AGxPlayerController* GxPC = GxCharacter->GetGxPlayerController();
	gxcheck(GxPC);

	// input is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	float MoveX = MovementVector.X;
	float MoveY = MovementVector.Y;

	const FRotator YawRotation(0.0f, GxPC->GetControlRotation().Yaw, 0.0f);

	if (MoveX != 0.0f)
	{
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GxCharacter->AddMovementInput(RightDirection, MoveX);
	}

	if (MoveY != 0.0f)
	{
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GxCharacter->AddMovementInput(ForwardDirection, MoveY);
	}
}

void UGxHeroComponent::Input_Look(const FInputActionValue& InputActionValue)
{
	AGxCharacter* GxCharacter = GetPawn<AGxCharacter>();
	gxcheck(GxCharacter);

	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	float LookX = LookAxisVector.X;
	float LookY = LookAxisVector.Y;

	if (LookX != 0.0f)
	{
		GxCharacter->AddControllerYawInput(LookX);
	}

	if (LookY != 0.0f)
	{
		GxCharacter->AddControllerPitchInput(LookY);
	}
}
