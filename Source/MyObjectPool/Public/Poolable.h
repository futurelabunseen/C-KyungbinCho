// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "Poolable.generated.h"

UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * IPoolable
 * 
 * IPoolable 인터페이스를 구현한 액터는 오브젝트 풀로 관리될 수 있다.
 */
class MYOBJECTPOOL_API IPoolable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void OnSpawnFromPool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void OnReturnToPool();
};
