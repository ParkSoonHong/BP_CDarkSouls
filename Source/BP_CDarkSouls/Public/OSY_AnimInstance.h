// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OSY_PursuerFSM.h"
#include "OSY_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BP_CDARKSOULS_API UOSY_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FSM")
	EEnmeyState animState;

	//RushAttack할지말지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bRushAttackPlay =false;
	//러쉬 어택이 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable,Category="FSMEVENT")
	void OnRushAttackEndAnim();
	//-----------------------------------------------------------------
	//Attack1_1을 할지말지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack1_1Play = false;
	// Attack1_1이 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable,Category="FSMEVENT")
	void OnAttack1_1EndAnim();
	//------------------------------------------------------------------
	//Attack1-2를 할지말지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack1_2Play = false;
	// Attack1_2가 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable,Category="FSMEVENT")
	void OnAttack1_2EndAnim();
	//-----------------------------------------------------------------
	//Attack1-3를 할지말지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack1_3Play = false;
	// Attack1_2가 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable,Category="FSMEVENT")
	void OnAttack1_3EndAnim();
	//------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack2Play = false;
	// Attack1_2가 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable,Category="FSMEVENT")
	void OnAttack2EndAnim();
	//----------------------------------------
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bDiePlay = false;


};