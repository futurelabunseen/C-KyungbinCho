// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * FGxGameModule
 * 
 * 주 게임 모듈
 */
class FGxGameModule : public FDefaultGameModuleImpl
{
	//~FDefaultGameModuleImpl
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
	//~End of FDefaultGameModuleImpl interface
};

IMPLEMENT_PRIMARY_GAME_MODULE(FGxGameModule, GirlzXtreme, "GirlzXtreme");
