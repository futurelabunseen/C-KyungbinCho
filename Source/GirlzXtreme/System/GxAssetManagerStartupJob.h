// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"

#include "GxAssetManagerStartupJob.generated.h"

//////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE_OneParam(FGxAssetManagerStartupJobSubstepProgress, float /*NewProgress*/);

//////////////////////////////////////////////////////////////////////

/**
 * FGxAssetManagerStartupJob
 *
 * Handles reporting progress from streamable handles
 */
USTRUCT()
struct FGxAssetManagerStartupJob
{
	GENERATED_BODY()

	FGxAssetManagerStartupJobSubstepProgress SubstepProgressDelegate;
	TFunction<void(const FGxAssetManagerStartupJob&, TSharedPtr<FStreamableHandle>&)> JobFunc;
	FString JobName;
	float JobWeight;
	mutable double LastUpdate = 0;

	/** Default ctor */
	FGxAssetManagerStartupJob()
	{
	}

	/** Simple job that is all synchronous */
	FGxAssetManagerStartupJob(const FString& InJobName, const TFunction<void(const FGxAssetManagerStartupJob&, TSharedPtr<FStreamableHandle>&)>& InJobFunc, float InJobWeight)
		: JobFunc(InJobFunc)
		, JobName(InJobName)
		, JobWeight(InJobWeight)
	{
	}

	/** Perform actual loading, will return a handle if it created one */
	TSharedPtr<FStreamableHandle> DoJob() const;

	void UpdateSubstepProgress(float NewProgress) const
	{
		SubstepProgressDelegate.ExecuteIfBound(NewProgress);
	}

	void UpdateSubstepProgressFromStreamable(TSharedRef<FStreamableHandle> StreamableHandle) const
	{
		if (SubstepProgressDelegate.IsBound())
		{
			// StreamableHandle::GetProgress traverses() a large graph and is quite expensive
			double Now = FPlatformTime::Seconds();
			if (LastUpdate - Now > 1.0 / 60)
			{
				SubstepProgressDelegate.Execute(StreamableHandle->GetProgress());
				LastUpdate = Now;
			}
		}
	}
};
