// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * GxGameplayTags
 *
 * 게임에서 사용되는 태그를 정의
 */
namespace GxGameplayTags
{
	/* [TODO]
	// Declare all of the custom native tags that Gx will use
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
	*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Jumping);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Crouching);
};
