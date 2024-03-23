// Fill out your copyright notice in the Description page of Project Settings.

#include "GxGameMode.h"

#include "GxLogChannels.h"
#include "UObject/ConstructorHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GxGameMode)

//////////////////////////////////////////////////////////////////////////
// AGxGameMode

AGxGameMode::AGxGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Assets/ThirdPerson/Blueprints/BP_ThirdPersonCharacter_C"));
	gxcheck(PlayerPawnBPClass.Class);

	DefaultPawnClass = PlayerPawnBPClass.Class;
}
