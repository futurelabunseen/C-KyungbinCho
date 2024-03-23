// Fill out your copyright notice in the Description page of Project Settings.

#include "GxAssetManagerStartupJob.h"

#include "GxLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxAssetManagerStartupJob)

//////////////////////////////////////////////////////////////////////////
// FGxAssetManagerStartupJob

TSharedPtr<FStreamableHandle> FGxAssetManagerStartupJob::DoJob() const
{
	const double JobStartTime = FPlatformTime::Seconds();

	TSharedPtr<FStreamableHandle> Handle;
	GX_LOG(Display, TEXT("Startup job \"%s\" starting"), *JobName);
	JobFunc(*this, Handle);

	if (Handle.IsValid())
	{
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateRaw(this, &FGxAssetManagerStartupJob::UpdateSubstepProgressFromStreamable));
		Handle->WaitUntilComplete(0.0f, false);
		Handle->BindUpdateDelegate(FStreamableUpdateDelegate());
	}

	GX_LOG(Display, TEXT("Startup job \"%s\" took %.2f seconds to complete"), *JobName, FPlatformTime::Seconds() - JobStartTime);

	return Handle;
}
