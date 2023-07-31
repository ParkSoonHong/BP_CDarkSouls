// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PSH_CPlayer.h"
#include "Components/CapsuleComponent.h"
#include "PSH_Shield.h"

UPlayerAnim::UPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempRoll(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Roll.MT_PSH_Roll_C'"));
	if (tempRoll.Succeeded())
	{
		RollMontage = tempRoll.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempRRoll(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_RRoll.MT_PSH_RRoll'"));
	if (tempRRoll.Succeeded())
	{
		RRollMontage = tempRRoll.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempLRoll(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_LRoll.MT_PSH_LRoll'"));
	if (tempLRoll.Succeeded())
	{
		LRollMontage = tempLRoll.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempBRoll(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_BRoll.MT_PSH_BRoll'"));
	if (tempBRoll.Succeeded())
	{
		BRollMontage = tempBRoll.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempBackStep(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_BackStep.MT_PSH_BackStep_C'"));
	if (tempBackStep.Succeeded())
	{
		BackStepMontage = tempBackStep.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempAttack(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/AttackAnim/MT_PSH_Attack1.MT_PSH_Attack1_C'"));
	if (tempAttack.Succeeded())
	{
		AttackMontage = tempAttack.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempAttack2(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Attack2.MT_PSH_Attack2_C'"));
	if (tempAttack.Succeeded())
	{
		AttackMontage2 = tempAttack2.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempHardAttack(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_HArdAttack.MT_PSH_HArdAttack_C'"));
	if (tempHardAttack.Succeeded())
	{
		HardAttackMontage = tempHardAttack.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempDamage1(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/Hit/Hit.Hit_C'"));
	if (tempDamage1.Succeeded())
	{
		DamageMontage = tempDamage1.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempDamage2(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/hit2.hit2'"));
	if (tempDamage2.Succeeded())
	{
		DamageMontage2 = tempDamage2.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempDamage3(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/hit3.hit3'"));
	if (tempDamage3.Succeeded())
	{
		DamageMontage3 = tempDamage3.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempLife(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/NewFolder/Mon_Drink.Mon_Drink'"));
	if (tempLife.Succeeded())
	{
		healingMontage = tempLife.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempEquip(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/equip.equip'"));
	if (tempEquip.Succeeded())
	{
		equipMontage = tempEquip.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempUnEquip(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/unequip.unequip'"));
	if (tempUnEquip.Succeeded())
	{
		unequipMontage = tempUnEquip.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempDefenseOn(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Shild.MT_PSH_Shild'"));
	if (tempDefenseOn.Succeeded())
	{
		DefenseOnMontage = tempDefenseOn.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempParry(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Parry.MT_PSH_Parry_C'"));
	if (tempParry.Succeeded())
	{
		parryMontage = tempParry.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempDie(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_Dead.MT_Dead'"));
	if (tempDie.Succeeded())
	{
		deadMontage = tempDie.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempStun(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Stun.MT_PSH_Stun'"));
	if (tempStun.Succeeded())
	{
		StunMontage = tempStun.Object;
	}
}
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto ownerPwan = TryGetPawnOwner();
	// 2. TPSPlayer가 필요하다.
	Player = Cast<APSH_CPlayer>(ownerPwan);

	if (Player != nullptr)
	{

		FVector Right = Player->GetActorRightVector();
		// 3. foward vector 가 필요하다.
		FVector forward = Player->GetActorForwardVector();
		// 4. 속도가 필요하다.
		FVector velocity = Player->GetVelocity();
		// 5. 나를 소유하고 있는 tpsPlayer의 속도와 foward 내적값을 할당하고싶다.
		speed = FVector::DotProduct(forward, velocity);

		diretion = FVector::DotProduct(Right, velocity);

		// isinAir 값을 활당하고 싶다
		//auto movement = Player->GetCharacterMovement();
		//cppisinAir = movement->IsFalling();

	}

}

void UPlayerAnim::AnimNotify_AttackEnd() // 어택 끝났어
{
	Player->PlayingAttack = false;
	Player->isAttackTime = true;
	Player->iscombo = false;
	Player->comboCount = 0;
	Player->isRoll = true;
	Player->isBackStep = true;
	Player->isAttack = true;
	Player->isHearing = true;
	Player->isDefense = true;
	Player->isRest = true;
	Player->SworldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPlayerAnim::AnimNotify_StartAttack() // 공격 시작
{
	Player->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player->PlayingAttack = true;
	Player->isAttackTime = false;
	Player->isAttack = false;
	Player->isRoll=false;
	Player->isBackStep=false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;
}

void UPlayerAnim::AnimNotify_ComboAttack() // 콤보 할거야?
{
	Player->PlayingAttack = true;
	Player->isAttackTime = true;
	Player->iscombo = true;

}

void UPlayerAnim::AnimNotify_BackstetpTime()
{
	Player->isAttack = true;
	Player->isAttackTime = true;
	Player->isBackStep = true;
	Player->isRoll = true;
	Player->isHearing = true;
	Player->isDefense = true;
	Player->isRest = true;
	Montage_Stop(0.5f,BackStepMontage);
}



void UPlayerAnim::AnimNotify_RollEnd()
{
	Player->isRoll = true;
	Player->isAttack = true;
	Player->isAttackTime = true;
	Player->isBackStep = true;
	Player->isHearing = true;
	Player->isDefense = true;
	Player->isRest = true;
}

void UPlayerAnim::AnimNotify_endHardAttack()
{
	Player->isAttack = true;
	Player->isAttackTime = true;
	Player->PlayingAttack = false;
	Player->isAttack = true;
	Player->isRoll = true;
	Player->isBackStep = true;
	Player->isHearing = true;
	Player->isDefense = true;
	Player->isRest = true;
	Player->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void UPlayerAnim::AnimNotify_HitEnd()
{
	Player->isAttack = true;
	Player->isRoll = true;
	Player->isBackStep = true;
	Player->isAttackTime = true;
	Player->isMoving = true;
	Player->isHearing = true;
	Player->isDefense = true;
}

void UPlayerAnim::AnimNotify_endHeling()
{
	Player->isAttackTime = true;
	Player->isAttack = true;
	Player->isRoll=true;
	Player->isBackStep = true;;
	Player->isHearing = true;
	Player->isDefense = true;
}

void UPlayerAnim::AnimNotify_movingOn()
{
	Player->isMoving = true;
}

void UPlayerAnim::AnimNotify_Equip()
{
	Player->compSword->SetVisibility(true);
	Player->shield->SetVisibility(true);
}

void UPlayerAnim::AnimNotify_UnEquip()
{
	Player->compSword->SetVisibility(false);
	Player->shield->SetVisibility(false);
	
}

void UPlayerAnim::AnimNotify_endEquip()
{
	Player->isAttack = true;
	Player->isAttackTime = true;
	Player->isBackStep = true;
	Player->isRoll = true;
	Player->isMoving = true;
	Player->isHearing = true;
	Player->isDefense = true;
}

void UPlayerAnim::AnimNotify_ParryEnd()
{
	Player->isParry = false;
	Player->shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPlayerAnim::AnimNotify_stunEnd()
{
	Player->isAttack = true;
	Player->isAttackTime = true;
	Player->isBackStep = true;
	Player->isRoll = true;
	Player->isMoving = true;
	Player->isHearing = true;
	Player->isDefense = true;
}

void UPlayerAnim::PlayRollAnimation()
{
	Montage_Play(RollMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->PlayingAttack = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;
}

void UPlayerAnim::PlayRRollAnimation()
{
	Montage_Play(RRollMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->PlayingAttack = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;
}

void UPlayerAnim::PlayLRollAnimation()
{
	Montage_Play(LRollMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->PlayingAttack = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;
}

void UPlayerAnim::PlayBRollAnimation()
{
	Montage_Play(BRollMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->PlayingAttack = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;
}	

void UPlayerAnim::PlayBackStepAnimation() // 백스텝 실행 
{ 
	Montage_Play(BackStepMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = true;
	Player->isRoll = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;

}


void UPlayerAnim::PlayDamgedAnimation()
{
	Montage_Play(DamageMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->isRoll = false;
	Player->isMoving = false;
	Player->isHearing = false;
/*	Player->isDefense = false;*/
}

void UPlayerAnim::PlayDamgedAnimation2()
{
	Montage_Play(DamageMontage2);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->isRoll = false;
	Player->isMoving = false;
	Player->isHearing = false;
/*	Player->isDefense = false;*/
}

void UPlayerAnim::PlayDamgedAnimation3()
{
	Montage_Play(DamageMontage3);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->isRoll = false;
	Player->isMoving = false;
	Player->isHearing = false;
// 	Player->isDefense = false;
}

void UPlayerAnim::PlayAttackAnimation()
{
	Montage_Play(AttackMontage);
	UE_LOG(LogTemp,Warning,TEXT("Attack1"));
}

void UPlayerAnim::PlayAttackAnimation2()
{
	Montage_Play(AttackMontage2);
}


void UPlayerAnim::PlayHardAttackAnimation()
{
	Montage_Play(HardAttackMontage);
	Player->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player->PlayingAttack = true;
	Player->isAttack = false;
	Player->isRoll = false;
	Player->isBackStep = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isRest = false;
}

void UPlayerAnim::PlayDefenseOnAnimation()
{
	Montage_Play(DefenseOnMontage);
	Player->isRest = false;
}

void UPlayerAnim::PlayParryAnimation()
{
	Montage_Play(parryMontage);
}

void UPlayerAnim::PlayHealingAnimation()
{
	Montage_Play(healingMontage);
	Player->isMoving = false;
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isRoll = false;
	Player->isBackStep = false;
	Player->isHearing = false;
	Player->isDefense = false;
}

void UPlayerAnim::PlayDeadAnimation()
{
	Montage_Play(deadMontage);
}

void UPlayerAnim::PlayEquipAnimation() // 무기 장착
{
	Montage_Play(equipMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->isRoll = false;
	Player->isMoving = false;
	Player->isHearing = false;
	Player->isDefense = true;
	Player->isEquip = true;
}

void UPlayerAnim::PlayUnEquipAnimation()
{
	Montage_Play(unequipMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->isRoll = false;
	Player->isMoving = false;
	Player->isHearing = false;
	Player->isDefense = false;
	Player->isEquip = false;
}

void UPlayerAnim::PlayStunAnimation()
{
	Montage_Play(StunMontage);
	Player->isAttack = false;
	Player->isAttackTime = false;
	Player->isBackStep = false;
	Player->isRoll = false;
	Player->isMoving = false;
	Player->isHearing = false;
	Player->isDefense = false;
}

