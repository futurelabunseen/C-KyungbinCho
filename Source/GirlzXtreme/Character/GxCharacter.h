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
class UGxAnimInstance;
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

	UFUNCTION(BlueprintCallable, Category = "Gx|Character")
	UGxAnimInstance* GetGxAnimInstance() const;

protected:
	//~ACharacter interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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
	// [TODO] Weapon Component 같은 것으로 빼는 게 좋을 것 같다.
	// 기본 무기 클래스들이며 BeginPlay() 에서 Hidden 상태로 캐릭터에 모두 장착된다.
	UPROPERTY(EditDefaultsOnly, Category = "Gx|Weapon")
	TArray<TSubclassOf<AGxWeapon>> DefaultWeaponClasses;
	// BeginPlay() 에서 스폰돼 장착된 무기들을 저장
	// 각 GA 를 시전할 때 장착할 무기를 찾아야 하므로 TMap 사용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Weapon")
	TMap<TSubclassOf<AGxWeapon>, TObjectPtr<AGxWeapon>> EquippedWeapons;
	// 현재 사용중인 무기 리스트
	// 스킬을 중첩 사용할 때를 대비한 것
	// Tail 의 무기가 현재 보여지는 무기를 가리킨다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gx|Weapon")
	TArray<TObjectPtr<AGxWeapon>> EnabledWeaponList;

public:
	UFUNCTION(BlueprintCallable, Category = "Gx|Weapon")
	void EquipWeapons(const TArray<TSubclassOf<AGxWeapon>>& Weapons, bool bHiddenInGame = true);

	UFUNCTION(BlueprintCallable, Category = "Gx|Weapon")
	AGxWeapon* GetCurrentWeapon() const { return (0 < EnabledWeaponList.Num() ? EnabledWeaponList.Last() : nullptr); }

	UFUNCTION(BlueprintCallable, Category = "Gx|Weapon")
	AGxWeapon* EnableWeapon(TSubclassOf<AGxWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Gx|Weapon")
	bool DisableWeapon(AGxWeapon* Weapon);

public:
	//~ACharacter interface
	virtual bool CanCrouch() const override;
	//~End of ACharacter interface
};
