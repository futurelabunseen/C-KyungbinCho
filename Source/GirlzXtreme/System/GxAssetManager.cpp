// Fill out your copyright notice in the Description page of Project Settings.

#include "GxAssetManager.h"

#include "GxLogChannels.h"
#include "AbilitySystemGlobals.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAssetManager)

//////////////////////////////////////////////////////////////////////

#define GX_STARTUP_JOB_WEIGHTED(JobFunc, JobWeight) StartupJobs.Add(FGxAssetManagerStartupJob(#JobFunc, [this](const FGxAssetManagerStartupJob& StartupJob, TSharedPtr<FStreamableHandle>& LoadHandle){JobFunc;}, JobWeight))
#define GX_STARTUP_JOB(JobFunc) GX_STARTUP_JOB_WEIGHTED(JobFunc, 1.f)

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// UGxAssetManager

UGxAssetManager::UGxAssetManager(const FObjectInitializer& ObjectInitializer)
{
}

UGxAssetManager& UGxAssetManager::Get()
{
	check(GEngine);

	if (UGxAssetManager* Singleton = Cast<UGxAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	GX_LOG(Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to GxAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UGxAssetManager>();
}

/**
 * UGxAssetManager::StartInitialLoading
 *
 * GAS 전역 데이터 초기화를 포함해, StartupJobs에 등록된 모든 작업을 실행한다.
 */
void UGxAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UGxAssetManager::StartInitialLoading");

	// This does all of the scanning, need to do this now even if loads are deferred
	Super::StartInitialLoading();

	GX_STARTUP_JOB(InitializeAbilitySystem());

	// Run all the queued up startup jobs
	DoAllStartupJobs();
}

void UGxAssetManager::InitializeAbilitySystem()
{
	SCOPED_BOOT_TIMING("UGxAssetManager::InitializeAbilitySystem");

	UAbilitySystemGlobals::Get().InitGlobalData();
}

/**
 * UGxAssetManager::DoAllStartupJobs
 * 
 * 각 작업에는 가중치가 설정되어 있다.
 * 먼저 모든 작업의 가중치 총합을 구한 후, 개별 작업이 끝날 때마다 가중치를 더하며 100%까지 채워가는 방식이다.
 * 
 * 걸린 시간이 아니라 미리 정한 가중치를 사용하는 이유는 실행 전에는 총 시간이 얼마나 걸릴지 알 수 없이 때문이다.
 * 마지막에 걸린 시간을 로그로 남긴다.
 */
void UGxAssetManager::DoAllStartupJobs()
{
	SCOPED_BOOT_TIMING("UGxAssetManager::DoAllStartupJobs");
	const double AllStartupJobsStartTime = FPlatformTime::Seconds();

	if (IsRunningDedicatedServer())
	{
		// No need for periodic progress updates, just run the jobs
		for (const FGxAssetManagerStartupJob& StartupJob : StartupJobs)
		{
			StartupJob.DoJob();
		}
	}
	else
	{
		if (StartupJobs.Num() > 0)
		{
			float TotalJobValue = 0.0f;
			for (const FGxAssetManagerStartupJob& StartupJob : StartupJobs)
			{
				TotalJobValue += StartupJob.JobWeight;
			}

			float AccumulatedJobValue = 0.0f;
			for (FGxAssetManagerStartupJob& StartupJob : StartupJobs)
			{
				const float JobValue = StartupJob.JobWeight;
				StartupJob.SubstepProgressDelegate.BindLambda([This = this, AccumulatedJobValue, JobValue, TotalJobValue](float NewProgress)
					{
						const float SubstepAdjustment = FMath::Clamp(NewProgress, 0.0f, 1.0f) * JobValue;
						const float OverallPercentWithSubstep = (AccumulatedJobValue + SubstepAdjustment) / TotalJobValue;

						This->UpdateInitialGameContentLoadPercent(OverallPercentWithSubstep);
					});

				StartupJob.DoJob();

				StartupJob.SubstepProgressDelegate.Unbind();

				AccumulatedJobValue += JobValue;

				UpdateInitialGameContentLoadPercent(AccumulatedJobValue / TotalJobValue);
			}
		}
		else
		{
			UpdateInitialGameContentLoadPercent(1.0f);
		}
	}

	StartupJobs.Empty();

	GX_NET_LOG_SCREEN_TICK(FColor::White, TEXT("All startup jobs took %.2f seconds to complete"), FPlatformTime::Seconds() - AllStartupJobsStartTime);
}

void UGxAssetManager::UpdateInitialGameContentLoadPercent(float GameContentPercent)
{
	// Could route this to the early startup loading screen
}
