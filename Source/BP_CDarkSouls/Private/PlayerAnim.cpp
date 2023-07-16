// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PSH_CPlayer.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto ownerPwan = TryGetPawnOwner();
	// 2. TPSPlayer�� �ʿ��ϴ�.
	auto Player = Cast<APSH_CPlayer>(ownerPwan);

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
	Montage_Play(AttackMontage);
}
