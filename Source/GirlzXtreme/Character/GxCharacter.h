// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/GxAbilityInputID.h"

#include "GxCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class AGxPlayerController;
class AGxPlayerState;
class UAbilitySystemComponent;
class UGxAbilitySystemComponent;
class UGxGameplayAbility;
class UGameplayEffect;
struct FInputActionValue;

/**
 * AGxCharacter
 * 
 * 게임에서 기본으로 사용되는 캐릭터 클래스
 * 모듈형 게임플레이와 함께 사용될 수 있다.
 */
UCLASS(Meta = (ShortTooltip = "The base character pawn class used by this project."))
class AGxCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** Movement Input Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MovementMappingContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	/** Combat Input Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CombatMappingContext;
	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;
	/** Skill1 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill1Action;
	/** Skill2 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill2Action;
	/** Ultimate Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gx|Input|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UltimateAction;

public:
	AGxCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Gx|Character")
	AGxPlayerController* GetGxPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Gx|Character")
	AGxPlayerState* GetGxPlayerState() const;

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	UFUNCTION(BlueprintCallable, Category = "Gx|Character")
	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
protected:
	//~ACharacter interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual bool CanJumpInternal_Implementation() const override;
	//~End of ACharacter interface

	virtual void SetupGASInputComponent();
	void GASInputPressed(EGxAbilityInputID InputId);
	void GASInputReleased(EGxAbilityInputID InputId);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gx|Ability")
	TArray<TSubclassOf<UGxGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gx|Ability")
	TArray<TSubclassOf<UGxGameplayAbility>> DefaultInputAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gx|Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

public:
	UFUNCTION(BlueprintCallable, Category = "Gx|Ability")
	void GiveAbilities(const TArray<TSubclassOf<UGxGameplayAbility>>& Abilities) const;

	UFUNCTION(BlueprintCallable, Category = "Gx|Effect")
	void ApplyEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects) const;

	//~ACharacter interface
	virtual void PawnClientRestart() override;
	virtual bool CanCrouch() const override;
	//~End of ACharacter interface
};
