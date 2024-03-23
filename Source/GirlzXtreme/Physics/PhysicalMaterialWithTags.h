// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameplayTagContainer.h"

#include "PhysicalMaterialWithTags.generated.h"

/**
 * UPhysicalMaterialWithTags
 *
 * 태그를 담을 수 있는 Physical Material
 */
UCLASS(Meta = (ShortTooltip = "Physical Material with tags"))
class UPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPhysicalMaterialWithTags(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// A container of gameplay tags that game code can use to reason about this physical material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PhysicalProperties)
	FGameplayTagContainer Tags;
};
