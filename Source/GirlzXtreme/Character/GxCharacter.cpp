// Fill out your copyright notice in the Description page of Project Settings.

#include "GxCharacter.h"

#include "GxLogChannels.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Player/GxPlayerController.h"
#include "Player/GxPlayerState.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/GxGameplayAbility.h"
#include "AbilitySystem/Abilities/GxAbilityInputID.h"
#include "GameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxCharacter)

//////////////////////////////////////////////////////////////////////

#define BIND_IA(InputAction, TriggerEvent, Callback, ...) \
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::##TriggerEvent##, this, Callback, ##__VA_ARGS__)
#define BIND_GAS_IA_PRESSED(InputAction, TriggerEvent, GxAbilityInputID, ...) \
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::##TriggerEvent##, this, &ThisClass::GASInputPressed, EGxAbilityInputID::##GxAbilityInputID##, ##__VA_ARGS__)
#define BIND_GAS_IA_RELEASED(InputAction, TriggerEvent, GxAbilityInputID, ...) \
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::##TriggerEvent##, this, &ThisClass::GASInputReleased, EGxAbilityInputID::##GxAbilityInputID##, ##__VA_ARGS__)

//////////////////////////////////////////////////////////////////////

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

	GiveAbilities(DefaultAbilities);
	GiveAbilities(DefaultInputAbilities);
	ApplyEffects(DefaultEffects);
}

void AGxCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Add Input Mapping Context
	AGxPlayerController* GxPlayerController = Cast<AGxPlayerController>(Controller);
	gxcheck(GxPlayerController);

	GxPlayerController->AddMappingContext(MovementMappingContext, /*Priority*/1);
	GxPlayerController->AddMappingContext(CombatMappingContext, /*Priority*/0);
}

void AGxCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

AGxPlayerController* AGxCharacter::GetGxPlayerController() const
{
	return CastChecked<AGxPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AGxPlayerState* AGxCharacter::GetGxPlayerState() const
{
	return CastChecked<AGxPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UGxAbilitySystemComponent* AGxCharacter::GetGxAbilitySystemComponent() const
{
	return Cast<UGxAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AGxCharacter::GetAbilitySystemComponent() const
{
	AGxPlayerState* GxPS = GetGxPlayerState();
	gxcheck(GxPS, nullptr);

	return GxPS->GetAbilitySystemComponent();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Moving
	BIND_IA(MoveAction, Triggered, &ThisClass::Move);

	// Looking
	BIND_IA(LookAction, Triggered, &ThisClass::Look);

	SetupGASInputComponent();
}

void AGxCharacter::SetupGASInputComponent()
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Jumping
	BIND_GAS_IA_PRESSED(JumpAction, Started, Jump);
	BIND_GAS_IA_RELEASED(JumpAction, Completed, Jump);

	// Crouching
	BIND_GAS_IA_PRESSED(CrouchAction, Started, Crouch);
	BIND_GAS_IA_RELEASED(CrouchAction, Completed, Crouch);

	// Attack
	BIND_GAS_IA_PRESSED(AttackAction, Started, Attack);
	BIND_GAS_IA_RELEASED(AttackAction, Completed, Attack);

	// Skill1
	BIND_GAS_IA_PRESSED(Skill1Action, Started, Skill1);
	BIND_GAS_IA_RELEASED(Skill1Action, Completed, Skill1);

	// Skill2
	BIND_GAS_IA_PRESSED(Skill2Action, Started, Skill2);
	BIND_GAS_IA_RELEASED(Skill2Action, Completed, Skill2);

	// Ultimate
	BIND_GAS_IA_PRESSED(UltimateAction, Started, Ultimate);
	BIND_GAS_IA_RELEASED(UltimateAction, Completed, Ultimate);
}

void AGxCharacter::GASInputPressed(EGxAbilityInputID InputId)
{
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	FGameplayAbilitySpec* GASpec = GxASC->FindAbilitySpecFromInputID(ToUtype(InputId));
	gxcheck(GxASC);
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

void AGxCharacter::GASInputReleased(EGxAbilityInputID InputId)
{
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	FGameplayAbilitySpec* GASpec = GxASC->FindAbilitySpecFromInputID(ToUtype(InputId));
	gxcheck(GxASC);
	gxcheck(GASpec);

	GASpec->InputPressed = false;
	if (GASpec->IsActive())
	{
		GxASC->AbilitySpecInputReleased(*GASpec);
	}
}

void AGxCharacter::Move(const FInputActionValue& Value)
{
	gxcheck(Controller);

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AGxCharacter::Look(const FInputActionValue& Value)
{
	gxcheck(Controller);

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

bool AGxCharacter::CanJumpInternal_Implementation() const
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	gxcheck(MovementComponent, false);

	bool bCanJump = Super::CanJumpInternal_Implementation();

	return (bCanJump || (!MovementComponent->IsFalling() && bIsCrouched));
}

bool AGxCharacter::CanCrouch() const
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	gxcheck(MovementComponent, false);

	bool bCanCrouch = Super::CanCrouch();

	return (bCanCrouch && !MovementComponent->IsFalling());
}

//////////////////////////////////////////////////////////////////////////
// GAS

void AGxCharacter::GiveAbilities(const TArray<TSubclassOf<UGxGameplayAbility>>& Abilities) const
{
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxASC);

	for (const auto& Ability : Abilities)
	{
		FGameplayAbilitySpec StartSpec(Ability);
		const UGxGameplayAbility* AbilityCDO = Ability.GetDefaultObject();
		bool bIsInputAbility = (AbilityCDO->GetGxAbilityInputID() != EGxAbilityInputID::None);

		if (bIsInputAbility)
		{
			EGxAbilityInputID InputID = Ability.GetDefaultObject()->GetGxAbilityInputID();
			StartSpec.InputID = ToUtype(InputID);
		}
		GxASC->GiveAbility(StartSpec);
	}
}

void AGxCharacter::ApplyEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects) const
{
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponent();
	gxcheck(GxASC);

	FGameplayEffectContextHandle EffectContext = GxASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const auto& Effect : Effects)
	{
		FGameplayEffectSpecHandle NewGEHandle = GxASC->MakeOutgoingSpec(Effect, /*Level*/1, EffectContext);
		gxcheck(NewGEHandle.IsValid());

		GxASC->ApplyGameplayEffectSpecToSelf(*NewGEHandle.Data.Get());
	}
}
