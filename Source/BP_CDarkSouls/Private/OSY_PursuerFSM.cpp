// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_PursuerFSM.h"
#include "Templates/Casts.h"
#include "UPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "OSY_Pursuer.h"
#include "../BP_CDarkSouls.h"

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
	case EEnmeyState::Dash:
		DashState();
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

	// 플레이어와의 거리를 탐지하다가 대시거리가되면 대시상태로 넘어가고, 대시거리보다 짧으면 무브상태로 넘어가고, 어택거리가 되면 어택으로 넘어간다
	// 필요속성 : 플레이어와의 거리, 대시거리, 무브거리,어택거리
	// 
	// 플레이어와의 거리를 탐지한다
	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	// 만약 플레이어와의 거리가 대시거리보다 멀다면
	if (distance < DashDistance)
	{
		// 대시상태로 넘어가고
		mState = EEnmeyState::Dash;
		PRINT_LOG(TEXT("DASH!!!"));
	}
	// 그렇지 않고 대시거리보다 짧고 어택거리보다 긴 무브거리가 되면
	else if (distance < MoveDistance)
	{
	// 무브상태로 넘어가고
		mState = EEnmeyState::Move;
		PRINT_LOG(TEXT("MOVE!!!"));
	}
	// 둘다 아니고 어택거리보다 짧아지면
	else
	{
	// 어택 상태로 넘어가
		mState = EEnmeyState::Attack;
		PRINT_LOG(TEXT("Attack!!!"));

	}
	

}
// 타겟쪽으로 이동하고 싶다.
// 필요속성 : 타겟, 이동속도
void UOSY_PursuerFSM::MoveState()
{
// 	거리를 탐지하고 있다
		FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
		//float distance = Direction.Length();
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

void UOSY_PursuerFSM::DashState()
{

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

