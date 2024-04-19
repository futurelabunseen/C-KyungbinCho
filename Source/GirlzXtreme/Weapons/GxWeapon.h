// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "GxWeapon.generated.h"

class AGxCharacter;
// [TODO]
class USkeletalMeshComponent;

// Delegate used to broadcast weapon event.
// 장착, 탈착 등
DECLARE_MULTICAST_DELEGATE_OneParam(FGxWeaponEvent, AGxCharacter* /*OwningActor*/);

/**
 * AGxWeapon
 *
 * 게임에서 사용되는 무기 Base
 * 무기는 기본적으로 장착 가능하다고 가정한다.
 */
UCLASS(Abstract, Meta = (ShortTooltip = "The base weapon class used by this project."))
class AGxWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AGxWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	virtual void Equip(AGxCharacter* NewOwnerCharacter);
	virtual void Unequip();

	FGxWeaponEvent OnEquipped;
	FGxWeaponEvent OnUnequipped;

protected:
	// [TODO] StaticMesh, SkeletalMesh 를 한번에 관리하는 방법?
	// 무기 메시
	UPROPERTY(VisibleAnywhere, Category = "Gx|Weapon")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	// 장착 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gx|Weapon")
	FName AttachSocket;
};
