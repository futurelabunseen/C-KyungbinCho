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
// 표현식이 무효하면 로그 출력 => 즉시 반환
#define gxcheck(Expr, /*Return value when Expr is invalid*/...)		{ if (!(Expr)) { GX_LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }
// 표현식이 무효하면 로그만 출력
#define gxcheck_log(Expr)		{ if (!(Expr)) { GX_LOG(Error, TEXT("INVALID : %s"), TEXT("'"#Expr"'")); } }

DECLARE_LOG_CATEGORY_EXTERN(LogGxNet, Log, All);

#define GX_LOCALROLEINFO GX_ROLE_TO_TEXT(GetLocalRole())
#define GX_REMOTEROLEINFO GX_ROLE_TO_TEXT(GetRemoteRole())
#define GX_SUBLOCALROLEINFO GX_ROLE_TO_TEXT(GetOwner()->GetLocalRole())
#define GX_SUBREMOTEROLEINFO GX_ROLE_TO_TEXT(GetOwner()->GetRemoteRole())
#define GX_ROLE_TO_TEXT(Role) ((Role == ENetRole::ROLE_Authority) ? TEXT("Auth") : (Role == ENetRole::ROLE_AutonomousProxy) ? TEXT("Auto") : (Role == ENetRole::ROLE_SimulatedProxy) ? TEXT("Simu") : TEXT("None"))

#define GX_NETMODEINFO GX_NETMODE_TO_TEXT(GetNetMode()) 
#define GX_NETMODE_TO_TEXT(NetMode) ((NetMode == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client %d"), GPlayInEditorID) : (NetMode == ENetMode::NM_DedicatedServer) ? TEXT("DEDICATED") : (NetMode == ENetMode::NM_ListenServer) ? TEXT("LISTEN") : TEXT("STANDALONE"))

#define GX_NET_LOG(Verbosity, Format, ...) UE_LOG(LogGxNet, Verbosity, TEXT("[%s]\t[%s / %s] %s %s"), GX_NETMODEINFO, GX_LOCALROLEINFO, GX_REMOTEROLEINFO, *GX_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define GX_NET_SUBLOG(Verbosity, Format, ...) UE_LOG(LogGxNet, Verbosity, TEXT("[%s]\t[%s / %s] %s %s"), GX_NETMODEINFO, GX_SUBLOCALROLEINFO, GX_SUBREMOTEROLEINFO, *GX_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
