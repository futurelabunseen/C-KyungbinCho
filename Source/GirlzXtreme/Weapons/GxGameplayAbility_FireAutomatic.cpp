// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameplayAbility_FireAutomatic.h"

#include "GxLogChannels.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "AbilitySystem/GxAbilitySystemComponent.h"
#include "Character/GxCharacter.h"
#include "Weapons/GxGASGun.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameplayAbility_FireAutomatic)

//////////////////////////////////////////////////////////////////////////
// UGxGameplayAbility_FireAutomatic

UGxGameplayAbility_FireAutomatic::UGxGameplayAbility_FireAutomatic(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TimeBetweenShots(0.1f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGxGameplayAbility_FireAutomatic::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	// [TODO]
	// 스킬을 시전할 때 무기가 바뀌는 어빌리티들은, 여기 PreActivate() 에서 무기를 바꿔줄 예정이다.
	// 무기를 사용하는 스킬, 사용하지 않는 스킬도 나눠야 하나..?
	UE_DEBUG_BREAK();
}

void UGxGameplayAbility_FireAutomatic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// [TODO] 임시 코드
	UE_DEBUG_BREAK();
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);
	AGxGASGun* GxGASGun = Cast<AGxGASGun>(GxCharacter->GetCurrentWeapon());
	gxcheck(GxGASGun);

	int32 CurrentAmmo = GxGASGun->GetAmmo();
	UAbilityTask_Repeat* ReapeatTask = UAbilityTask_Repeat::RepeatAction(this, TimeBetweenShots, CurrentAmmo);
	gxcheck(ReapeatTask);

	ReapeatTask->OnPerformAction.AddDynamic(this, &ThisClass::OnPerformActionCallback);
	ReapeatTask->OnFinished.AddDynamic(this, &ThisClass::OnFinishedCallback);
	ReapeatTask->ReadyForActivation();
}

void UGxGameplayAbility_FireAutomatic::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEndAbility*/true, /*bWasCancelled*/false);
}

void UGxGameplayAbility_FireAutomatic::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGxGameplayAbility_FireAutomatic::OnPerformActionCallback(int32 ActionNumber)
{
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponentFromActorInfo();
	gxcheck(GxASC);
	
	// [TODO] 임시 코드
	UE_DEBUG_BREAK();
	if (GxASC->TryActivateAbilityByClass(FireSemiAutoAbility))
	{
		ApplyGameplayEffectToGun(FireCostEffect);

		AGxGASGun* GxGASGun = GetGxGASGun();
		gxcheck(GxGASGun);

		int32 CurrentAmmo = GxGASGun->GetAmmo();
		GX_NET_LOG_SCREEN_TICK(FColor::White, TEXT("Ammo: %d"), CurrentAmmo);
	}
}

void UGxGameplayAbility_FireAutomatic::OnFinishedCallback(int32 ActionNumber)
{
	return;

	// [TODO] 재장전
	UE_DEBUG_BREAK();
	GX_NET_LOG_SCREEN(FColor::White, TEXT("Reloading ..."));

	ApplyGameplayEffectToGun(ReloadEffect);

	// [TODO] 임시 코드
	UE_DEBUG_BREAK();
	AGxGASGun* GxGASGun = GetGxGASGun();
	int32 CurrentAmmo = GxGASGun->GetAmmo();
	UAbilityTask_Repeat* ReapeatTask = UAbilityTask_Repeat::RepeatAction(this, TimeBetweenShots, CurrentAmmo);
	gxcheck(ReapeatTask);

	ReapeatTask->OnPerformAction.AddDynamic(this, &ThisClass::OnPerformActionCallback);
	ReapeatTask->OnFinished.AddDynamic(this, &ThisClass::OnFinishedCallback);
	ReapeatTask->ReadyForActivation();
}

// [TODO] 임시 코드
void UGxGameplayAbility_FireAutomatic::ApplyGameplayEffectToGun(const TSubclassOf<UGameplayEffect>& EffectClass)
{
	UE_DEBUG_BREAK();
	UGxAbilitySystemComponent* GxASC = GetGxAbilitySystemComponentFromActorInfo();
	gxcheck(GxASC);
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter);
	UGxAbilitySystemComponent* GxGunASC = GetGxGunASC();
	gxcheck(GxGunASC);

	FGameplayEffectContextHandle EffectContext = GxASC->MakeEffectContext();
	EffectContext.AddSourceObject(GxCharacter);

	FGameplayEffectSpecHandle NewGEHandle = GxGunASC->MakeOutgoingSpec(EffectClass, /*Level*/1, EffectContext);
	gxcheck(NewGEHandle.IsValid());

	GxASC->ApplyGameplayEffectSpecToTarget(*NewGEHandle.Data.Get(), GxGunASC);
}

AGxGASGun* UGxGameplayAbility_FireAutomatic::GetGxGASGun()
{
	AGxCharacter* GxCharacter = GetGxCharacterFromActorInfo();
	gxcheck(GxCharacter, nullptr);
	
	return Cast<AGxGASGun>(GxCharacter->GetCurrentWeapon());
}

UGxAbilitySystemComponent* UGxGameplayAbility_FireAutomatic::GetGxGunASC()
{
	AGxGASGun* GxGASGun = GetGxGASGun();

	return (GxGASGun ? GxGASGun->GetGxAbilitySystemComponent() : nullptr);
}
