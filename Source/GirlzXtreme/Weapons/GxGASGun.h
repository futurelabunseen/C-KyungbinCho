// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/GxGASWeapon.h"

#include "GxGASGun.generated.h"

class UGxAmmoSet;
struct FOnAttributeChangeData;

/**
 * AGxGASGun
 *
 * 게임에서 사용되는 GAS 와 상호작용 가능한 총
 * 하나의 GAS 액터이고, OwningActor 와 AvatarActor 모두 자신이다.
 * GxAmmoSet 을 속성으로 갖는다.
 */
UCLASS(Meta = (ShortTooltip = "The GAS gun class used by this project. It has ammo attribute set."))
class AGxGASGun : public AGxGASWeapon
{
	GENERATED_BODY()
	
public:
	AGxGASGun(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gx|GASGun")
	int32 GetAmmo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gx|GASGun")
	int32 GetMaxAmmo();

private:
	// The ammo attribute set sub-object used by GAS guns.
	UPROPERTY(VisibleAnywhere, Category = "Gx|GASGun")
	TObjectPtr<UGxAmmoSet> AmmoSet;

	void OnAttributeChangedCallback(const FOnAttributeChangeData& AttributeChangedData) const;

	FDelegateHandle OnAmmoAttributeChangeHandle;
};
