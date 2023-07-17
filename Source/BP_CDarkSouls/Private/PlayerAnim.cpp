// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PSH_CPlayer.h"

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
