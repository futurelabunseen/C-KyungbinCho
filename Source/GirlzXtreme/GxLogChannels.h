// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Logging/LogMacros.h"

/**
 * GxLogChannels
 *
 * 로그 카테고리, 디버깅용 매크로를 선언한 헤더
 */

DECLARE_LOG_CATEGORY_EXTERN(LogGx, Log, All);

// 호출 함수 정보 문자열
#define GX_CALLINFO			(FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// 호출 함수 정보 출력
#define GX_LOG_CALLINFO(Verbosity)		UE_LOG(LogGx, Verbosity, TEXT("%s"), *GX_CALLINFO)
// 로그 + 호출 함수 정보 출력
#define GX_LOG(Verbosity, Format, ...)	UE_LOG(LogGx, Verbosity, TEXT("%s %s"), *GX_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
// 표현식이 무효하면 로그 출력
#define gxcheck(Expr)		{ if (!(Expr)) { GX_LOG(Error, TEXT("INVALID : %s"), TEXT("'"#Expr"'")); } }
// gxcheck + 즉시 반환
#define gxcheck_ret(Expr, /*Return value when Expr is invalid*/...)		{ if (!(Expr)) { GX_LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }
