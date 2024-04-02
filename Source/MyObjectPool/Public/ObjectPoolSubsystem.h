// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "ObjectPoolSubsystem.generated.h"

/**
 * FPoolArray
 * 
 * 하나의 클래스에 대한 오브젝트 풀
 */
USTRUCT(BlueprintType)
struct MYOBJECTPOOL_API FPoolArray
{
	GENERATED_BODY()

	FPoolArray()
	{
		ObjectPool.Reserve(200);
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool Subsystem")
	TArray<TObjectPtr<AActor>> ObjectPool;
};

/**
 * UObjectPoolSubsystem
 * 
 * 월드 서브시스템으로 구현한 싱글톤 오브젝트 풀 관리 객체
 */
UCLASS()
class MYOBJECTPOOL_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Pool Subsystem", Meta = (DeterminesOutputType = "PoolClass", DynamicOutputParam = "OutSpawnedActor"))
	void SpwanFromPool(const TSubclassOf<AActor> PoolClass, const FVector& Location, const FRotator& Rotation, AActor*& OutSpawnedActor);

	// UFUNCTION() 으로 지정하면 함수 템플릿으로 만들 수 없다.
	template <class ActorClass>
	ActorClass* SpwanFromPoolInternal(const TSubclassOf<AActor> PoolClass, const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "Pool Subsystem")
	void ReturnToPool(AActor* Poolable);

private:
	// TMap<클래스, 클래스에 대한 오브젝트 풀> 프로퍼티
	// TMap 에는 TArray 를 직접 넣을 수 없다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool Subsystem", Meta = (AllowPrivateAccess = "true"))
	TMap<TObjectPtr<UClass>, FPoolArray> ObjectPools;
};

template <class ActorClass>
ActorClass* UObjectPoolSubsystem::SpwanFromPoolInternal(const TSubclassOf<AActor> PoolClass, const FVector& Location, const FRotator& Rotation)
{
	ActorClass* PooledActor = nullptr;

	if (bool bImplementsPoolable = PoolClass.Get()->ImplementsInterface(UPoolable::StaticClass()))
	{
		FPoolArray& PoolArray = ObjectPools.FindOrAdd(PoolClass);
		auto& ObjectPool = PoolArray.ObjectPool;

		if (ObjectPool.IsEmpty())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			PooledActor = GetWorld()->SpawnActor<ActorClass>(PoolClass, Location, Rotation, SpawnParams);

			//static int32 NumSpawned = 0;
			//UE_LOG(LogTemp, Warning, TEXT("Spawn %d"), ++NumSpawned);
		}
		else
		{
			PooledActor = CastChecked<ActorClass>(ObjectPool.Pop(false));
			PooledActor->SetActorLocationAndRotation(Location, Rotation);
			
			//int32 NumElements = ObjectPool.Num();
			//int32 PoolCapacity = ObjectPool.GetAllocatedSize() / ObjectPool.GetTypeSize();
			//UE_LOG(LogTemp, Warning, TEXT("Reuse %d / %d"), NumElements, PoolCapacity);
		}

		IPoolable::Execute_OnSpawnFromPool(PooledActor);
	}

	return PooledActor;
}
