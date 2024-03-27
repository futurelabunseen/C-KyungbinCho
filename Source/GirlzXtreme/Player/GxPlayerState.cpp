// Fill out your copyright notice in the Description page of Project Settings.

#include "GxPlayerState.h"

#include "GxLogChannels.h"
#include "Player/GxPlayerController.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GxCharacterSet.h"
#include "Character/GxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxPlayerState)

//////////////////////////////////////////////////////////////////////////
// AGxPlayerState

AGxPlayerState::AGxPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGxAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CharacterSet = ObjectInitializer.CreateDefaultSubobject<UGxCharacterSet>(this, TEXT("CharacterSet"));
}

AGxPlayerController* AGxPlayerState::GetGxPlayerController() const
{
	return Cast<AGxPlayerController>(GetOwner());
}

UAbilitySystemComponent* AGxPlayerState::GetAbilitySystemComponent() const
{
	return GetGxAbilitySystemComponent();
}

void AGxPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	gxcheck(AbilitySystemComponent);
	gxcheck(CharacterSet);

	// Subscribe to UGxCharacterSet::WalkSpeed change
	auto& OnWalkSpeedAttributeChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterSet->GetWalkSpeedAttribute());
	OnWalkSpeedAttributeChangeHandle = OnWalkSpeedAttributeChange.AddUObject(this, &ThisClass::HandleAttributeChanged);
	// Subscribe to UGxCharacterSet::CrouchedSpeed change
	auto& OnCroucnedSpeedAttributeChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterSet->GetCrouchedSpeedAttribute());
	OnCrouchedSpeedAttributeChangeHandle = OnCroucnedSpeedAttributeChange.AddUObject(this, &ThisClass::HandleAttributeChanged);
}

void AGxPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	gxcheck(AbilitySystemComponent);
	gxcheck(CharacterSet);

	// Unsubscribe to UGxCharacterSet::WalkSpeed change
	auto& OnWalkSpeedAttributeChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterSet->GetWalkSpeedAttribute());
	OnWalkSpeedAttributeChange.Remove(OnWalkSpeedAttributeChangeHandle);
	// Unsubscribe to UGxCharacterSet::CrouchedSpeed change
	auto& OnCroucnedSpeedAttributeChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterSet->GetCrouchedSpeedAttribute());
	OnCroucnedSpeedAttributeChange.Remove(OnCrouchedSpeedAttributeChangeHandle);

	Super::EndPlay(EndPlayReason);
}

void AGxPlayerState::HandleAttributeChanged(const FOnAttributeChangeData& AttributeChangedData)
{
	AGxCharacter* GxCharacter = Cast<AGxCharacter>(GetPawn());
	gxcheck(GxCharacter);
	UCharacterMovementComponent* MovemenetComponent = GxCharacter->GetCharacterMovement();
	gxcheck(MovemenetComponent);

	FGameplayAttribute ChangedAttribute = AttributeChangedData.Attribute;
	float NewValue = AttributeChangedData.NewValue;

	if (ChangedAttribute == CharacterSet->GetWalkSpeedAttribute())
	{
		MovemenetComponent->MaxWalkSpeed = NewValue;
	}
	else if (ChangedAttribute == CharacterSet->GetCrouchedSpeedAttribute())
	{
		MovemenetComponent->MaxWalkSpeedCrouched = NewValue;
	}
}
