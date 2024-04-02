// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectPoolSubsystem.h"

#include "Poolable.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ObjectPoolSubsystem)

//////////////////////////////////////////////////////////////////////////
// UObjectPoolSubsystem

void UObjectPoolSubsystem::SpwanFromPool(const TSubclassOf<AActor> PoolClass, const FVector& Location, const FRotator& Rotation, AActor*& OutSpawnedActor)
{
	OutSpawnedActor = SpwanFromPoolInternal<AActor>(PoolClass, Location, Rotation);
}

void UObjectPoolSubsystem::ReturnToPool(AActor* PooledActor)
{
	check(PooledActor);

	const UClass* PoolableClass = PooledActor->GetClass();

	if (bool bImplementsPoolable = PoolableClass->ImplementsInterface(UPoolable::StaticClass()))
	{
		IPoolable::Execute_OnReturnToPool(PooledActor);
		FPoolArray& PoolArray = ObjectPools.FindChecked(PoolableClass);
		auto& ObjectPool = PoolArray.ObjectPool;

		ObjectPool.Add(PooledActor);

		//int32 NumElements = ObjectPool.Num();
		//int32 PoolCapacity = ObjectPool.GetAllocatedSize() / ObjectPool.GetTypeSize();
		//UE_LOG(LogTemp, Warning, TEXT("Return %d / %d"), NumElements, PoolCapacity);
	}
	else
	{
		PooledActor->Destroy();

		//UE_LOG(LogTemp, Warning, TEXT("Destroy"));
	}
}
