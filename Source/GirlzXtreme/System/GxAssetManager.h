// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "System/GxAssetManagerStartupJob.h"

#include "GxAssetManager.generated.h"

/**
 * UGxAssetManager
 *
 * Game implementation of the asset manager that overrides functionality and stores game-specific types.
 * It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 * This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 */
UCLASS()
class UGxAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UGxAssetManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Returns the AssetManager singleton object.
	static UGxAssetManager& Get();

protected:
	//~UAssetManager interface
	virtual void StartInitialLoading() override;
	//~End of UAssetManager interface

private:
	// Flushes the StartupJobs array. Processes all startup work.
	void DoAllStartupJobs();

	// Sets up the ability system
	void InitializeAbilitySystem();

	// Called periodically during loads, could be used to feed the status to a loading screen
	void UpdateInitialGameContentLoadPercent(float GameContentPercent);

	/**
	 * 확실하지 않은 내용
	 * 
	 * UPROPERTY 로 지정해준 TMap, TArray 에 들어가는 타입은 그 타입도 UCLASS() 나 USTRUCT() 로 지정해줘야 했다.
	 * 또, 기본 생성자도 있어야 했다.
	 * 
	 * UPROPERTY 로 지정하지 않으면 전방 선언을 통한 불완전 타입도 허용됐다.
	 */
	// The list of tasks to execute on startup. Used to track startup progress.
	UPROPERTY()
	TArray<FGxAssetManagerStartupJob> StartupJobs;
};
