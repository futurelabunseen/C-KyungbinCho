// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"

#include "GxCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGxHeroComponent;
class UInputComponent;
class UGxInputConfig;
class AGxPlayerController;
class AGxPlayerState;
// [TODO] 임시 코드
class AGxWeapon;
class UGxAbilitySystemComponent;
class UGxGameplayAbility;
class UGameplayEffect;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	/** Gx Hero component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGxHeroComponent> HeroComponent;

public:
	/** Input Config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<const UGxInputConfig> InputConfig;

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
	//~ACharacter interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual bool CanJumpInternal_Implementation() const override;
	//~End of ACharacter interface
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Ability")
	TArray<TSubclassOf<UGxGameplayAbility>> DefaultInputAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Ability")
	TArray<TSubclassOf<UGxGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns HeroComponent subobject **/
	FORCEINLINE UGxHeroComponent* GetGxHeroComponent() const { return HeroComponent; }

	// [TODO] 임시 코드
	UPROPERTY(EditDefaultsOnly, Category = "Gx|Weapon")
	TSubclassOf<AGxWeapon> DefaultWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Weapon")
	TObjectPtr<AGxWeapon> CurrentWeapon;
public:
	UFUNCTION(BlueprintCallable, Category = "Gx|Weapon")
	AGxWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

public:
	//~ACharacter interface
	virtual bool CanCrouch() const override;
	//~End of ACharacter interface
};
