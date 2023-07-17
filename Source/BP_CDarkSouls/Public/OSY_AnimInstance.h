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
	bool bRushAttackPlay = false;
	//러쉬 어택이 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable,Category="FSMEVENT")
	void OnRushAttackEndAnim();

	

	
};