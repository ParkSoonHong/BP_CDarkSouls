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

	UFUNCTION(BlueprintImplementableEvent,Category="FSMEvent")
	void PlayRushAttackAnim(FName sectionName); 

};