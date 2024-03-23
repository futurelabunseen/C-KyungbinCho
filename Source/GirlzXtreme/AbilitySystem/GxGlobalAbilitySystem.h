// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Templates/SubclassOf.h"
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"

#include "GxGlobalAbilitySystem.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UGxAbilitySystemComponent;

/**
 * FGlobalAppliedAbilityList
 * 
 * 모든 ASC 에 적용된 GA 를 관리하기 위한 구조체
 */
USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UGxAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UGxAbilitySystemComponent* ASC);
	void RemoveFromASC(UGxAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

/**
 * FGlobalAppliedEffectList
 *
 * 모든 ASC 에 적용된 GE 를 관리하기 위한 구조체
 */
USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UGxAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UGxAbilitySystemComponent* ASC);
	void RemoveFromASC(UGxAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

/**
 * FGlobalAppliedEffectList
 *
 * 모든 ASC 를 모아 관리하기 위한 월드 서브시스템
 */
UCLASS()
class UGxGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UGxGlobalAbilitySystem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gx")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gx")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gx")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Gx")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	/** Register an ASC with global system and apply any active global effects/abilities. */
	void RegisterASC(UGxAbilitySystemComponent* ASC);

	/** Removes an ASC from the global system, along with any active global effects/abilities. */
	void UnregisterASC(UGxAbilitySystemComponent* ASC);

private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<UGxAbilitySystemComponent>> RegisteredASCs;
};
