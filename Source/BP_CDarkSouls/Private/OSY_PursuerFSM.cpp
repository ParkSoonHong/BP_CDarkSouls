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
	case EEnmeyState::Walk:
		WalkState();
	case EEnmeyState::Rush:
		RushState();
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

	// 플레이어와의 거리를 탐지하다가 워크스타트보다 크면 러시로 넘어가고, 어택스타드보다 크면 무브상태로 넘어가고, 백스텝스타드보다 크면 어택으로 넘어가고 모두 아니면 백스텝해
	// 필요속성 : 플레이어와의 거리, 워크스타트거리, 어택스타트거리,백스텝스타트거리
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

	
	// 만약 플레이어와의 거리가 워크스타트보다 크면
	if (distance > WalkStartDistance)
	{
		// 대시상태로 넘어가고
		PRINT_LOG(TEXT("Go to Rush!!!"));
		mState = EEnmeyState::Rush;
	}
	// 그렇지 않고 어택보다 크면 워크로가
	else if (distance > AttackStartDistance)
	{
	// 무브상태로 넘어가고
		mState = EEnmeyState::Walk;
		PRINT_LOG(TEXT("Go to Walk!!!"));
	}
	// 둘다 아니고 어택거리보다 짧아지면
	else if(distance > BackstepStartDistance)
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
	FVector P = me->GetActorLocation()+FVector::ForwardVector*-1* Walkspeed *GetWorld()->DeltaRealTimeSeconds;
	mState = EEnmeyState::Idle;
}


// 플레이어와의 거리가 무브거리 안쪽이면 걸어가다가 어택 거리가 되면 아이들상태로 돌아간다

// 필요속성 : 타겟, 이동속도
void UOSY_PursuerFSM::WalkState()
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
	if (distance > AttackStartDistance)
	{
	//2. 무브를 실행하고
		FVector P= me->GetActorLocation()+Direction* Walkspeed *GetWorld()->DeltaRealTimeSeconds;
		me->SetActorLocation(P);
		PRINT_LOG(TEXT("Walk!!!"));
	}
	//3. 그렇지 않다면
	else
	{
	//4. 아이들로 돌아가
		PRINT_LOG(TEXT("M_Back to Idle!!!"));

	}
}

void UOSY_PursuerFSM::RushState()
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
	
	// 만약 플레이어와의 거리가 Rush거리보다 크면 대쉬를 실행하고,아이들로 돌아가
	if (distance > WalkStartDistance)
	{
	// 대시를 실행하는데
		FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
		me->SetActorLocation(P);
	PRINT_LOG(TEXT("Rush!!!"));

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
	if (distance > BackstepStartDistance)
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
	PRINT_LOG(TEXT("%f"),AttackRandom);

	if (AttackRandom > 60)
	{
		//PRINT_LOG(TEXT("Go to Attack1!!!"));
		mAttackState=EEnmeyAttackState::Attack1;

	}
	else if (AttackRandom > 30)
	{
		//PRINT_LOG(TEXT("Go to Attack2!!!"));
		mAttackState=EEnmeyAttackState::Attack2;
	}
	else
	{
		//PRINT_LOG(TEXT("Go to Attack3!!!"));
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

	// 이쪽으로 상태가 넘어오면 어택1을 하고
		//PRINT_LOG(TEXT("ATTACK1!!!!"));

	// 만약 어택거리 보다 크다면 아이들로 돌아가
		if (distance > AttackStartDistance)
		{
		//PRINT_LOG(TEXT("A1_Back to Idle"));
		 mState = EEnmeyState::Idle;
		}
	//  아직 어택거리면 어택베이직으로 돌아가
		else
		{
		//PRINT_LOG(TEXT("A1_Back to Attack Basic"));
		mAttackState= EEnmeyAttackState::AttackBasic;

		}





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


	// 이쪽으로 상태가 넘어오면 어택1을 하고
	//PRINT_LOG(TEXT("ATTACK2!!!!"));

		// 만약 어택거리 보다 크다면 아이들로 돌아가
		if (distance > AttackStartDistance)
		{
			//PRINT_LOG(TEXT("A2_Back to Idle"));
				mState = EEnmeyState::Idle;
		}
	//  아직 어택거리면 어택베이직으로 돌아가
		else
		{
			//PRINT_LOG(TEXT("A2_Back to Attack Basic"));
			mAttackState = EEnmeyAttackState::AttackBasic;

		}
	
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

	// 이쪽으로 상태가 넘어오면 어택1을 하고
	PRINT_LOG(TEXT("ATTACK3!!!!"));

		// 만약 어택거리 보다 크다면 아이들로 돌아가
		if (distance > AttackStartDistance)
		{
			PRINT_LOG(TEXT("A3_Back to Idle"));
				mState = EEnmeyState::Idle;
		}
	//  아직 어택거리면 어택베이직으로 돌아가
		else
		{
			PRINT_LOG(TEXT("A3_Back to Attack Basic"));
			mAttackState = EEnmeyAttackState::AttackBasic;

		}
}
