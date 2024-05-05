// Fill out your copyright notice in the Description page of Project Settings.

#include "GxCharacter.h"

#include "GxLogChannels.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/GxPlayerController.h"
#include "Player/GxPlayerState.h"
#include "Character/GxHeroComponent.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "Animation/GxAnimInstance.h"
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
	EquipWeapons(DefaultWeaponClasses);
	EnabledWeaponList.Add(EquippedWeapons[DefaultWeaponClasses[0]]);
	EnabledWeaponList[0]->SetActorHiddenInGame(false);
}

void AGxCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<AActor*> AttachedActors;

	GetAttachedActors(AttachedActors, false);

	for (AActor*& AttachedActor : AttachedActors)
	{
		AttachedActor->Destroy();
	}

	Super::EndPlay(EndPlayReason);
}

void AGxCharacter::EquipWeapons(const TArray<TSubclassOf<AGxWeapon>>& Weapons, bool bHiddenInGame)
{
	UWorld* World = GetWorld();
	gxcheck(World);

	for (auto& WeaponClass : Weapons)
	{
		gxcheck(WeaponClass);

		auto& SpawnedWeapon = EquippedWeapons.Emplace(WeaponClass, World->SpawnActor<AGxWeapon>(WeaponClass));
		SpawnedWeapon->Equip(this);
		SpawnedWeapon->SetActorHiddenInGame(bHiddenInGame);
	}
}

AGxWeapon* AGxCharacter::EnableWeapon(TSubclassOf<AGxWeapon> WeaponClass)
{
	if (AGxWeapon* CurrentWeapon = GetCurrentWeapon())
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}
	
	AGxWeapon* NewWeapon = EquippedWeapons[WeaponClass];
	gxcheck(NewWeapon, nullptr);

	NewWeapon->SetActorHiddenInGame(false);
	EnabledWeaponList.Add(NewWeapon);

	return NewWeapon;
}

bool AGxCharacter::DisableWeapon(AGxWeapon* Weapon)
{
	gxcheck(Weapon, false);
	
	Weapon->SetActorHiddenInGame(true);
	bool bResult = (0 < EnabledWeaponList.RemoveSingle(Weapon));

	if (0 < EnabledWeaponList.Num())
	{
		AGxWeapon* LastWeapon = EnabledWeaponList.Last();
		gxcheck(LastWeapon, bResult);

		LastWeapon->SetActorHiddenInGame(false);
	}

	return bResult;
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

	return (GxPS ? GxPS->GetAbilitySystemComponent() : nullptr);
}

UGxAbilitySystemComponent* AGxCharacter::GetGxAbilitySystemComponent() const
{
	return Cast<UGxAbilitySystemComponent>(GetAbilitySystemComponent());
}

UGxAnimInstance* AGxCharacter::GetGxAnimInstance() const
{
	USkeletalMeshComponent* SkelMeshComponent = GetMesh();

	return (SkelMeshComponent ? Cast<UGxAnimInstance>(SkelMeshComponent->GetAnimInstance()) : nullptr);
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
