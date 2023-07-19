// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PSH_CPlayer.h"

UPlayerAnim::UPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempRoll(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Roll.MT_PSH_Roll_C'"));
	if (tempRoll.Succeeded())
	{
		RollMontage = tempRoll.Object;
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

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempShild(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Shild.MT_PSH_Shild_C'"));
	if (tempShild.Succeeded())
	{
		ShildMontage = tempShild.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempHardAttack(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_HArdAttack.MT_PSH_HArdAttack_C'"));
	if (tempHardAttack.Succeeded())
	{
		HardAttackMontage = tempHardAttack.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempParry(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Parry.MT_PSH_Parry_C'"));
	if (tempParry.Succeeded())
	{
		parryMontage = tempParry.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempDamage(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/Hit/Hit.Hit_C'"));
	if (tempDamage.Succeeded())
	{
		DamageMontage = tempDamage.Object;
	}


	ConstructorHelpers::FObjectFinder<UAnimMontage>tempLife(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/NewFolder/Mon_Drink.Mon_Drink_C'"));
	if (tempLife.Succeeded())
	{
		RifeTimeMontage = tempLife.Object;
	}

}
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto ownerPwan = TryGetPawnOwner();
	// 2. TPSPlayer�� �ʿ��ϴ�.
	Player = Cast<APSH_CPlayer>(ownerPwan);

	if (Player != nullptr)
	{

		FVector Right = Player->GetActorRightVector();
		// 3. foward vector �� �ʿ��ϴ�.
		FVector forward = Player->GetActorForwardVector();
		// 4. �ӵ��� �ʿ��ϴ�.
		FVector velocity = Player->GetVelocity();
		// 5. ���� �����ϰ� �ִ� tpsPlayer�� �ӵ��� foward �������� �Ҵ��ϰ�ʹ�.
		speed = FVector::DotProduct(forward, velocity);

		diretion = FVector::DotProduct(Right, velocity);

		// isinAir ���� Ȱ���ϰ� �ʹ�
		//auto movement = Player->GetCharacterMovement();
		//cppisinAir = movement->IsFalling();

	}

}

void UPlayerAnim::AnimNotify_AttackEnd() // ���� ������
{
	Player->PlayingAttack = true;
	Player->iscombo = false;
	Player->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPlayerAnim::AnimNotify_StartAttack() // ���� ����
{
	Player->PlayingAttack = false;
}

void UPlayerAnim::AnimNotify_ComboAttack() // �޺� �Ұž�?
{
	Player->PlayingAttack = true;
	Player->iscombo = true;
}

void UPlayerAnim::AnimNotify_BackstetpTime()
{
	Player->isBackStep = false;
}



void UPlayerAnim::AnimNotify_RollEnd()
{
	Player->isRoll = false;
}

void UPlayerAnim::AnimNotify_endHardAttack()
{
	Player->isAttack = true;
	Player->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void UPlayerAnim::PlayRollAnimation()
{
	Montage_Play(RollMontage);
	
}

void UPlayerAnim::PlayBackStepAnimation()
{
	Montage_Play(BackStepMontage);
}


void UPlayerAnim::PlayDamgedAnimation()
{
	Montage_Play(DamageMontage);
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
// void UPlayerAnim::PlayAttackAnimation(FName sctionName)
// {
// 	Montage_JumpToSection(sctionName,AttackMontage);
// }

void UPlayerAnim::PlayHardAttackAnimation()
{
	Montage_Play(HardAttackMontage);
}

void UPlayerAnim::PlayShildAnimation()
{
	Montage_Play(ShildMontage);
}

void UPlayerAnim::PlayParryAnimation()
{
	Montage_Play(parryMontage);
}

void UPlayerAnim::PlayRifeTimeAnimation()
{
	Montage_Play(RifeTimeMontage);
}

