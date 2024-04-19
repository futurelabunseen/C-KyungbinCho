// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"

#include "GxPlayerState.generated.h"

class AGxPlayerController;
class UGxAbilitySystemComponent;
class UGxCharacterSet;
class UGxHealthSet;
class UGxSkillSet;
struct FOnAttributeChangeData;

/**
 * AGxPlayerState
 * 
 * 게임에서 사용되는 플레이어 스테이트
 * 모듈형 게임플레이와 함께 사용될 수 있다.
 */
UCLASS(Meta = (ShortTooltip = "The base player state class used by this project."))
class AGxPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGxPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~APlayerState interface
	virtual void PostInitializeComponents() override;
	//~End of APlayerState interface

	//~AModularPlayerState interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AModularPlayerState interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	UFUNCTION(BlueprintCallable, Category = "Gx|PlayerState")
	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable, Category = "Gx|PlayerState")
	AGxPlayerController* GetGxPlayerController() const;

private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Gx|PlayerState")
	TObjectPtr<UGxAbilitySystemComponent> AbilitySystemComponent;

	// The character attribute set sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Gx|PlayerState")
	TObjectPtr<UGxCharacterSet> CharacterSet;

	// The health attribute set sub-object used by damageable entities.
	UPROPERTY(VisibleAnywhere, Category = "Gx|PlayerState")
	TObjectPtr<UGxHealthSet> HealthSet;
	
	// The skill attribute set sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Gx|PlayerState")
	TObjectPtr<UGxSkillSet> SkillSet;

	void OnAttributeChangedCallback(const FOnAttributeChangeData& AttributeChangedData) const;

	FDelegateHandle OnWalkSpeedAttributeChangeHandle;
	FDelegateHandle OnCrouchedSpeedAttributeChangeHandle;
	FDelegateHandle OnHealthAttributeChangeHandle;
	FDelegateHandle OnUltimateGaugeAttributeChangeHandle;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gx|PlayerState")
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gx|PlayerState")
	int32 GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gx|PlayerState")
	float GetUltimateGauge() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gx|PlayerState")
	float GetMaxUltimateGauge() const;
};
