// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_PursuerFSM.h"
#include "Templates/Casts.h"
#include "UPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "OSY_Pursuer.h"
#include "../BP_CDarkSouls.h"
#include "Chaos/Vector.h"

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


	// FSM 목차
	switch (mState)
	{
	case EEnmeyState::Idle:
		IdleState();
		break;
	case EEnmeyState::Backstep:
		BackstepState();
		break;
	case EEnmeyState::Move:
		MoveState();
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
	// Attack SubMachin 목차
}

void UOSY_PursuerFSM::IdleState()
{

	// 플레이어와의 거리를 탐지하다가 대시거리가되면 대시상태로 넘어가고, 대시거리보다 짧으면 무브상태로 넘어가고, 어택거리가 되면 어택으로 넘어간다
	// 필요속성 : 플레이어와의 거리, 대시거리, 무브거리,어택거리
	// 
	// 플레이어와의 거리를 탐지한다
	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());


	// 만약 플레이어와의 거리가 대시거리보다 멀다면
	if (distance > DashDistance)
	{
		// 대시상태로 넘어가고
		PRINT_LOG(TEXT("Go to DASH!!!"));
		mState = EEnmeyState::Dash;
	}
	// 그렇지 않고 대시거리보다 짧고 어택거리보다 긴 무브거리가 되면
	else if (distance > MoveDistance)
	{
	// 무브상태로 넘어가고
		mState = EEnmeyState::Move;
		PRINT_LOG(TEXT("Go to MOVE!!!"));
	}
	// 둘다 아니고 어택거리보다 짧아지면
	else if(distance > AttackDistance)
	{
	// 어택 상태로 넘어가
		mState = EEnmeyState::Attack;
		PRINT_LOG(TEXT("Go to Attack!!!"));
	}
	else
	{
		mState = EEnmeyState::Backstep;
		PRINT_LOG(TEXT("Go to Beckstep!!!"));

	}

}
// 플레이어와의 거리가 백스텝 거리보다 작으면 백스텝을 실행하고 아이들상태로 돌아가
void UOSY_PursuerFSM::BackstepState()
{
	PRINT_LOG(TEXT("B_Back to Idle!!!"));
	mState = EEnmeyState::Idle;
}


// 플레이어와의 거리가 무브거리 안쪽이면 걸어가다가 어택 거리가 되면 아이들상태로 돌아간다

// 필요속성 : 타겟, 이동속도
void UOSY_PursuerFSM::MoveState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

	//만약 플레이어와의 거리가 무브거리보다 크면 무브하고, 그렇지 않다면 아이들로 돌아가
	//1. 만약 플레이어와의 거리가 무브거리보다 크면
	if (distance > MoveDistance)
	{
	//2. 무브를 실행하고
		PRINT_LOG(TEXT("MOVE!!!"));
	}
	//3. 그렇지 않다면
	else
	{
	//4. 아이들로 돌아가
		PRINT_LOG(TEXT("M_Back to Idle!!!"));

	}
}

void UOSY_PursuerFSM::DashState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());
	
	// 만약 플레이어와의 거리가 Dash거리보다 크면 대쉬를 실행하고,아이들로 돌아가
	//1. 만약 플레이어와의 거리가 대시거리보다 크면
	if (distance > DashDistance)
	{
	//2. 대시를 실행하고
	PRINT_LOG(TEXT("DASH!!!"));
	}
	//3. 그렇지 않으면 아이들로 돌아가
	else
	{
	PRINT_LOG(TEXT("D_Back to Idle!!!"));
		mState = EEnmeyState::Idle;
	}
}

void UOSY_PursuerFSM::AttackState()
{
	switch (mAttackState)
	{
		case EEnmeyAttackState::AttackBasic:
		{
		AttackBasic();
		break;
		}
		case EEnmeyAttackState::Attack1:
		{
			Attack1();
			break;
		case EEnmeyAttackState::Attack2:
		{
			Attack2();
			break;
		}
		case EEnmeyAttackState::Attack3:
		{
			Attack3();
			break;
		}
		default:
			break;
		}
	}
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

// 만약 플레이어와의 거리가 어택거리랑 짧으면 어택을 실행해, 그렇지 않으면 아이들로 돌아가
//1. 만약 플레이어와의 거리가 어
	if (distance > AttackDistance)
	{
		PRINT_LOG(TEXT("ATTACK!!!"));
		
	}
	else
	{
		PRINT_LOG(TEXT("A_Back To Idle!!!"));
		mState= EEnmeyState::Idle;
	  
	}


}

void UOSY_PursuerFSM::DamageState()
{

}

void UOSY_PursuerFSM::DieState()
{

}

/// <summary>
/// Attack 함수
/// </summary>
void UOSY_PursuerFSM::AttackBasic()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

	// 랜덤한 확률인데 40프로는 어택1로, 그다음 30은 어택2로, 마지막 30은 어택3으로 스테이트를 이동하고 싶다.
	// 필요속성 : 랜덤한 확률, 
	float AttackRandom = FMath::RandRange(1,100);

	if (AttackRandom > 60)
	{
		PRINT_LOG(TEXT("Go to Attack1!!!"));
		mAttackState=EEnmeyAttackState::Attack1;
	}
	else if (AttackRandom > 30)
	{
		PRINT_LOG(TEXT("Go to Attack2!!!"));
		mAttackState=EEnmeyAttackState::Attack2;
	}
	else
	{
		PRINT_LOG(TEXT("Go to Attack3!!!"));
		mAttackState=EEnmeyAttackState::Attack3;
	}
}

void UOSY_PursuerFSM::Attack1()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

	// 이쪽으로 상태가 넘어오면 어택1을 하고 어택베이직으로 돌아가
	PRINT_LOG(TEXT("1_Back to Attack Basic"));
	mAttackState= EEnmeyAttackState::AttackBasic;

}

void UOSY_PursuerFSM::Attack2()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

	PRINT_LOG(TEXT("2_Back to Attack Basic"));
	mAttackState = EEnmeyAttackState::AttackBasic;
}

void UOSY_PursuerFSM::Attack3()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// 타겟방향으로 회전하기
	// forward vector가 direction이 되고 싶다
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
	me->SetActorRotation(forward.Rotation());

	PRINT_LOG(TEXT("3_Back to Attack Basic"));
	mAttackState = EEnmeyAttackState::AttackBasic;
}

