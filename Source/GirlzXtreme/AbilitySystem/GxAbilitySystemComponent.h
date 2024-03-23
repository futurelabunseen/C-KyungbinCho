// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"

#include "GxAbilitySystemComponent.generated.h"

class AActor;

/**
 * UGxAbilitySystemComponent
 *
 * 게임에서 기본으로 사용되는 ASC
 * UGxGlobalAbilitySystem 이라는 월드 서브시스템이 ASC 를 모아서 관리할 수 있도록 한다.
 */
UCLASS()
class UGxAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGxAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~End of UAbilitySystemComponent interface
};
