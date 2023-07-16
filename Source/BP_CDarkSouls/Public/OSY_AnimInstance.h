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
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttackPlay= false;

	//러쉬 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bRushPlay = false;
	//러쉬 어택 상태를 재생할지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bRushAttackPlay = false;

	// 어택 1-1을 재생할지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack11Play = false;
	// 어택 1-2를 재생할지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack12Play = false;
	// 어택 1-3을 재생항지 여부
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="FSM")
	bool bAttack13Play = false;

	UFUNCTION(BlueprintImplementableEvent,Category="FSMEvent")
	void PlayRushAttackAnim(FName sectionName); 

	UFUNCTION(BlueprintImplementableEvent,Category="FSMEvent")
	void PlayAttack1Anim(FName sectionName);


	UFUNCTION()
	void ANimNotify_OnEndCombo1Event();
	UFUNCTION()
	void ANimNotify_OnEndCombo2Event();
	UFUNCTION()
	void ANimNotify_OnEndCombo3Event();
};