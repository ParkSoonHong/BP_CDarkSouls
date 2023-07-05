// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPSMsg, Log, All)

// 호출된 함수 위치, 줄번호까지 문자열로 가져오기
#define APPINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// 호출된 정보만 찍는 매크로
#define  PRINT_INFO() UE_LOG(TPSMsg,Warning,TEXT("%s"), *APPINFO)

// 주어진 포멧으로 메시지를 찍는 매크로
#define PRINT_LOG(fmt, ...) UE_LOG(TPSMsg,Warning,TEXT("%s %s"), *APPINFO, *FString::Printf(fmt,##__VA_ARGS__))
