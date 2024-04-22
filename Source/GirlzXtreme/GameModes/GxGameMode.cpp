// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameMode.h"

#include "GxLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameMode)

//////////////////////////////////////////////////////////////////////////
// AGxGameMode

AGxGameMode::AGxGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseSeamlessTravel = true;
}

void AGxGameMode::PostLogin(APlayerController* NewPlayer)
{
	GX_NET_LOG(Log, TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	gxcheck(NetDriver);

	if (NetDriver->ClientConnections.Num() == 0)
	{
		GX_NET_LOG(Log, TEXT("No Client Connection"));
	}

	for (const auto& Connection : NetDriver->ClientConnections)
	{
		GX_NET_LOG(Log, TEXT("Client Connections: %s"), *Connection->GetName());
	}

	GX_NET_LOG(Log, TEXT("End"));
}
