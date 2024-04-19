// Fill out your copyright notice in the Description page of Project Settings.

#include "GxWeapon.h"

#include "GxLogChannels.h"
#include "Character/GxCharacter.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxWeapon)

//////////////////////////////////////////////////////////////////////////
// AGxWeapon

AGxWeapon::AGxWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, AttachSocket(TEXT("weapon_socket"))
{
	// [TODO]
	// StaticMesh, SkeletalMesh 둘을 한 번에 관리하고 싶은데..
	// UMeshComponent 로 지정하면 생성자에서 Fatal Error 가 발생한다.
	UE_DEBUG_BREAK();
	SkeletalMeshComponent = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMeshComponent"));
	// 문제는 콜리전 프로파일을 설정하는 부분에서 발생한다.
	// 하지만 이 부분을 주석 처리하고 UMeshComponent 로 변경해도, BP 에서는 메시를 설정할 수 없다.
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	RootComponent = SkeletalMeshComponent;

	// Tick is disabled by default.
	PrimaryActorTick.bCanEverTick = false;
}

void AGxWeapon::SetActorHiddenInGame(bool bNewHidden)
{
	if (GetOwner() != nullptr)
	{
		Super::SetActorHiddenInGame(bNewHidden);
	}
}

void AGxWeapon::Equip(AGxCharacter* NewOwnerCharacter)
{
	gxcheck(NewOwnerCharacter);

	USkeletalMeshComponent* CharacterMesh = NewOwnerCharacter->GetMesh();
	gxcheck(CharacterMesh);

	if (CharacterMesh->DoesSocketExist(AttachSocket))
	{
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;

		AttachToComponent(CharacterMesh, AttachmentTransformRules, AttachSocket);
	}

	SetOwner(NewOwnerCharacter);
	OnEquipped.Broadcast(NewOwnerCharacter);
}

void AGxWeapon::Unequip()
{
	AGxCharacter* OwnerCharacter = Cast<AGxCharacter>(GetOwner());
	gxcheck(OwnerCharacter);

	USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
	gxcheck(CharacterMesh);

	if (CharacterMesh->DoesSocketExist(AttachSocket))
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	OnUnequipped.Broadcast(OwnerCharacter);
	SetOwner(nullptr);
}
