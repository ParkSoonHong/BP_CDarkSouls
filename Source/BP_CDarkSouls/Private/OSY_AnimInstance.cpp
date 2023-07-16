// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_AnimInstance.h"
#include "OSY_PursuerFSM.h"

void UOSY_AnimInstance::ANimNotify_OnEndCombo1Event()
{
	auto owner = TryGetPawnOwner();
	auto fsm = Cast<UOSY_PursuerFSM>(owner->GetDefaultSubobjectByName(TEXT("FSM")));
	if (fsm)
	{
	//fsm->OnEndCombo1Event();
	}
}

void UOSY_AnimInstance::ANimNotify_OnEndCombo2Event()
{
	auto owner = TryGetPawnOwner();
	auto fsm = Cast<UOSY_PursuerFSM>(owner->GetDefaultSubobjectByName(TEXT("FSM")));
	if (fsm)
	{
	//	fsm->OnEndCombo2Event();
	}
}

void UOSY_AnimInstance::ANimNotify_OnEndCombo3Event()
{
	auto owner = TryGetPawnOwner();
	auto fsm = Cast<UOSY_PursuerFSM>(owner->GetDefaultSubobjectByName(TEXT("FSM")));
	if (fsm)
	{
	//fsm->OnEndCombo3Event();
	}
}
