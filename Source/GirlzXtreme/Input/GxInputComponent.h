// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "GxLogChannels.h"
#include "Input/GxInputConfig.h"

#include "GxInputComponent.generated.h"

/**
 * UGxInputComponent
 *
 * UGxInputConfig 데이터 에셋을 통해 입력 매핑, 바인딩을 담당하는 컴포넌트
 */
UCLASS(Config = Input, Meta = (ShortTooltip = "Component used to manage input mappings and bindings using an input config data asset."))
class UGxInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UGxInputComponent(const FObjectInitializer& ObjectInitializer);

	template <class UserClass, typename FuncType>
	void BindNativeAction(const UGxInputConfig* InputConfig, EGxNativeInputType InputType, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UGxInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
};

//////////////////////////////////////////////////////////////////////////
// UGxInputComponent

template<class UserClass, typename FuncType>
void UGxInputComponent::BindNativeAction(const UGxInputConfig* InputConfig, EGxNativeInputType InputType, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	gxcheck(InputConfig);

	if (const UInputAction* NativeAction = InputConfig->NativeInputActions[InputType])
	{
		BindAction(NativeAction, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGxInputComponent::BindAbilityActions(const UGxInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	gxcheck(InputConfig);

	for (const auto& [InputID, AbilityAction] : InputConfig->AbilityInputActions)
	{
		bool bValidInputID = (InputID != EGxAbilityInputID::None);

		if (bValidInputID && AbilityAction)
		{
			gxcheck(PressedFunc);
			BindAction(AbilityAction, ETriggerEvent::Started, Object, PressedFunc, InputID);

			gxcheck(ReleasedFunc);
			BindAction(AbilityAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputID);
		}
	}
}
