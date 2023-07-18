// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_AnimInstance.h"
#include "OSY_PursuerFSM.h"
#include "OSY_Pursuer.h"



void UOSY_AnimInstance::OnRushAttackEndAnim()
{
	bRushAttackPlay= false;
}

void UOSY_AnimInstance::OnAttack1_1EndAnim()
{
	bAttack1_1Play = false;
}

void UOSY_AnimInstance::OnAttack1_2EndAnim()
{
	bAttack1_2Play = false;
}

void UOSY_AnimInstance::OnAttack1_3EndAnim()
{
	bAttack1_3Play = false;
}

void UOSY_AnimInstance::OnAttack2EndAnim()
{
	bAttack2Play = false;
}
