// Fill out your copyright notice in the Description page of Project Settings.

#include "GxCharacter.h"

#include "GxLogChannels.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/GxPlayerController.h"
#include "Player/GxPlayerState.h"
#include "Character/GxHeroComponent.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
// [TODO] 임시 코드
#include "Weapons/GxWeapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxCharacter)

//////////////////////////////////////////////////////////////////////////
// AGxCharacter

AGxCharacter::AGxCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a Gx Hero component
	HeroComponent = ObjectInitializer.CreateDefaultSubobject<UGxHeroComponent>(this, TEXT("HeroComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGxCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGxPlayerState* GxPS = GetGxPlayerState();
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxPS);
	gxcheck(GxASC);

	GxASC->InitAbilityActorInfo(GxPS, this);
	GxASC->GiveAbilities(DefaultInputAbilities);
	GxASC->GiveAbilities(DefaultAbilities);
	GxASC->ApplyEffects(DefaultEffects);
}

void AGxCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// [TODO] 임시 코드
	UE_DEBUG_BREAK();
	gxcheck(DefaultWeapon);
	CurrentWeapon = GetWorld()->SpawnActor<AGxWeapon>(DefaultWeapon);
	CurrentWeapon->Equip(this);
	CurrentWeapon->Unequip();
	CurrentWeapon->Equip(this);
}

AGxPlayerController* AGxCharacter::GetGxPlayerController() const
{
	return CastChecked<AGxPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AGxPlayerState* AGxCharacter::GetGxPlayerState() const
{
	return CastChecked<AGxPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UAbilitySystemComponent* AGxCharacter::GetAbilitySystemComponent() const
{
	AGxPlayerState* GxPS = GetGxPlayerState();
	gxcheck(GxPS, nullptr);

	return GxPS->GetAbilitySystemComponent();
}

UGxAbilitySystemComponent* AGxCharacter::GetGxAbilitySystemComponent() const
{
	return Cast<UGxAbilitySystemComponent>(GetAbilitySystemComponent());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	HeroComponent->InitializePlayerInput(PlayerInputComponent);
}

bool AGxCharacter::CanJumpInternal_Implementation() const
{
	if (Super::CanJumpInternal_Implementation())
	{
		return true;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	gxcheck(MovementComponent, false);

	return !MovementComponent->IsFalling();
}

bool AGxCharacter::CanCrouch() const
{
	if (!Super::CanCrouch())
	{
		return false;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	gxcheck(MovementComponent, false);

	return !MovementComponent->IsFalling();
}
