// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "GxAbilitySourceInterface.generated.h"

class UPhysicalMaterial;
struct FGameplayTagContainer;

/**
 * IGxAbilitySourceInterface
 *
 * 거리에 따른 데미지 감쇄 등을 계산하는 데 사용되는 인터페이스 클래스
 */
UINTERFACE(Meta = (ShortTooltip = "Base interface for anything acting as a ability calculation source."))
class UGxAbilitySourceInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IGxAbilitySourceInterface
{
	GENERATED_IINTERFACE_BODY()

	/**
	 * Compute the multiplier for effect falloff with distance
	 * 
	 * @param Distance			Distance from source to target for ability calculations (distance bullet traveled for a gun, etc...)
	 * @param SourceTags		Aggregated Tags from the source
	 * @param TargetTags		Aggregated Tags currently on the target
	 * 
	 * @return Multiplier to apply to the base attribute value due to distance
	 */
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const = 0;

	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const = 0;
};
