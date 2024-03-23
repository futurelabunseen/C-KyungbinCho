// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/ModuleManager.h"

/**
 * FGxGameModule
 * 
 * 주 게임 모듈
 */
class FGxGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FGxGameModule, GirlzXtreme, "GirlzXtreme");
