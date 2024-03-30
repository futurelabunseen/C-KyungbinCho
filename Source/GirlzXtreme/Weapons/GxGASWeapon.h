// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/GxWeapon.h"
#include "AbilitySystemInterface.h"

#include "GxGASWeapon.generated.h"

class UGxAbilitySystemComponent;
class UGxGameplayAbility;
class UGameplayEffect;

/**
 * AGxGASWeapon
 *
 * 게임에서 사용되는 GAS 와 상호작용 가능한 무기
 * 하나의 GAS 액터이고, OwningActor 와 AvatarActor 모두 자신이다.
 */
UCLASS(Meta = (ShortTooltip = "The GAS weapon class used by this project. This is a GAS Actor"))
class AGxGASWeapon : public AGxWeapon, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGxGASWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface
	UFUNCTION(BlueprintCallable, Category = "Gx|GASWeapon")
	UGxAbilitySystemComponent* GetGxAbilitySystemComponent() const { return AbilitySystemComponent; }

	//~AActor interface
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

protected:
	// The ability system component sub-object used by gas weapons.
	UPROPERTY(VisibleAnywhere, Category = "Gx|GASWeapon")
	TObjectPtr<UGxAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Ability")
	TArray<TSubclassOf<UGxGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
};
