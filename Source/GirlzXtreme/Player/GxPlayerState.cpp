// Fill out your copyright notice in the Description page of Project Settings.

#include "GxPlayerState.h"

#include "GxLogChannels.h"
#include "Player/GxPlayerController.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
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
	HealthSet = ObjectInitializer.CreateDefaultSubobject<UGxHealthSet>(this, TEXT("HealthSet"));
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
	gxcheck(HealthSet);

	// Subscribe to UGxCharacterSet::WalkSpeed change
	OnWalkSpeedAttributeChangeHandle = AbilitySystemComponent->AddOnAttributeChange(CharacterSet->GetWalkSpeedAttribute(), this, &ThisClass::OnAttributeChangedCallback);
	// Subscribe to UGxCharacterSet::CrouchedSpeed change
	OnCrouchedSpeedAttributeChangeHandle = AbilitySystemComponent->AddOnAttributeChange(CharacterSet->GetCrouchedSpeedAttribute(), this, &ThisClass::OnAttributeChangedCallback);
	// Subscribe to UGxHealthSet::Health change
	OnHealthAttributeChangeHandle = AbilitySystemComponent->AddOnAttributeChange(HealthSet->GetHealthAttribute(), this, &ThisClass::OnAttributeChangedCallback);
}

void AGxPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	gxcheck(AbilitySystemComponent);
	gxcheck(CharacterSet);
	gxcheck(HealthSet);

	// Unsubscribe to UGxCharacterSet::WalkSpeed change
	AbilitySystemComponent->RemoveOnAttributeChange(CharacterSet->GetWalkSpeedAttribute(), OnWalkSpeedAttributeChangeHandle);
	// Unsubscribe to UGxCharacterSet::CrouchedSpeed change
	AbilitySystemComponent->RemoveOnAttributeChange(CharacterSet->GetCrouchedSpeedAttribute(), OnCrouchedSpeedAttributeChangeHandle);
	// Unsubscribe to UGxHealthSet::Health change
	AbilitySystemComponent->RemoveOnAttributeChange(HealthSet->GetHealthAttribute(), OnHealthAttributeChangeHandle);

	Super::EndPlay(EndPlayReason);
}

// [TODO] 이 콜백에서 캐릭터에 직접 접근하지 말고, 델리게이트를 1번 더 써도 될 것 같다.
void AGxPlayerState::OnAttributeChangedCallback(const FOnAttributeChangeData& AttributeChangedData) const
{
	UE_DEBUG_BREAK();
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
	else if (ChangedAttribute == HealthSet->GetHealthAttribute())
	{
		// [TODO]
		UE_DEBUG_BREAK();
	}
}

int32 AGxPlayerState::GetHealth() const
{
	return StaticCast<int32>(HealthSet ? HealthSet->GetHealth() : -1);
}

int32 AGxPlayerState::GetMaxHealth() const
{
	return StaticCast<int32>(HealthSet ? HealthSet->GetMaxHealth() : -1);
}
