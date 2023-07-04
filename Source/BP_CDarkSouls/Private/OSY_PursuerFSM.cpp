// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_PursuerFSM.h"
#include "Templates/Casts.h"
#include "UPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "OSY_Pursuer.h"

// Sets default values for this component's properties
UOSY_PursuerFSM::UOSY_PursuerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOSY_PursuerFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Target = Cast<AUPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AUPlayer::StaticClass()));
	me = Cast<AOSY_Pursuer>(GetOwner());
	
}


// Called every frame
void UOSY_PursuerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// FSM 목차
	switch (mState)
	{
	case EEnmeyState::Idle:
		IdleState();
		break;
	case EEnmeyState::Move:
		MoveState();
		break;
	case EEnmeyState::Attack:
		AttackState();
		break;
	case EEnmeyState::Damage:
		DamageState();
		break;
	case EEnmeyState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void UOSY_PursuerFSM::IdleState()
{
	// 시간이 흐르다가
	currentTIme += GetWorld()->DeltaTimeSeconds;
	// 현재시간이 경과시간보다 커지면
	if (currentTIme > idleDelayTime)
	{
		// 이동으로 바꾸고 싶다
		mState = EEnmeyState::Move;
		currentTIme = 0;
	}

}
// 타겟쪽으로 이동하고 싶다.
// 필요속성 : 타겟, 이동속도
void UOSY_PursuerFSM::MoveState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	Direction.Normalize();

	FVector P = me->GetActorLocation() + Direction * speed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);

	Direction.Z = 0;
	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

}

void UOSY_PursuerFSM::AttackState()
{

}

void UOSY_PursuerFSM::DamageState()
{

}

void UOSY_PursuerFSM::DieState()
{

}

