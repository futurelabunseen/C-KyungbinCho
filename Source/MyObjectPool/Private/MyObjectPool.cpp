// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * FMyObjectPoolModule
 *
 * 주 플러그인 모듈
 */
class FMyObjectPoolModule : public IModuleInterface
{
public:
	FMyObjectPoolModule()
	{
	}

	//~IModuleInterface
	virtual void StartupModule() override
	{
		// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	}

	virtual void ShutdownModule() override
	{
		// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
		// we call this function before unloading the module.
	}
	//~End of IModuleInterface interface
};

IMPLEMENT_MODULE(FMyObjectPoolModule, MyObjectPool)
