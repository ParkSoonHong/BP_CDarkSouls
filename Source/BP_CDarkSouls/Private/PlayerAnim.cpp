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

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempAttack(TEXT("/Script/Engine.AnimMontage'/Game/ParkSoonHong/Ani/MT_PSH_Attck.MT_PSH_Attck_C'"));
	if (tempAttack.Succeeded())
	{
		AttackMontage = tempAttack.Object;
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


}
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto ownerPwan = TryGetPawnOwner();
	// 2. TPSPlayer가 필요하다.
	auto Player = Cast<APSH_CPlayer>(ownerPwan);

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

void UPlayerAnim::AnimNotify_Attack1()
{
	Attack1 = true;
}

void UPlayerAnim::AnimNotify_endHardAttack()
{
	hardAttackEnd = true;
	UE_LOG(LogTemp,Warning,TEXT("LastAttack"));
}

void UPlayerAnim::PlayRollAnimation()
{
	Montage_Play(RollMontage);
}

void UPlayerAnim::PlayBackStepAnimation()
{
	Montage_Play(BackStepMontage);
}

void UPlayerAnim::PlayAttackAnimation()
{
	Montage_Play(AttackMontage); // 변경필요
}

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


