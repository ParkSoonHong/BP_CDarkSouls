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
	case EEnmeyState::Rush:
		RushState();
		break;
	case EEnmeyState::Walk:
		WalkState();
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

	// 플레이어와의 거리를 탐지하다가 러시스타트보다 크면 무브로 넘어가고, 어택스타드보다 크면 러시상태로 넘어가고, 백스텝스타드보다 크면 어택으로 넘어가고 모두 아니면 백스텝해
	// 필요속성 : 플레이어와의 거리, 러시스타트거리, 어택스타트거리,백스텝스타트거리
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

	
	// 만약 플레이어와의 거리가 러시스타트보다 크면
	if (distance > RushStartDistance)
	{
		// 워크 상태로 넘어가고
		PRINT_LOG(TEXT("Go to Walk!!!"));
		mState = EEnmeyState::Walk;
	}
	// 그렇지 않고 어택거리보다 크면 러시로 가
	else if (distance > AttackStartDistance)
	{
	// 러시상태로 넘어가고
		currentTIme = 0;
		Rushspeed = 0;
		delyTime += GetWorld()->DeltaRealTimeSeconds * 100;
		UE_LOG(LogTemp,Error,TEXT("%f"),delyTime);
		delyTime = 0;
		mState = EEnmeyState::Rush;
		PRINT_LOG(TEXT("Go to Rush!!!"));
	}
	// 그렇지 않고 백스텝거리보다 크면 어택으로 가
	else if(distance > BackstepStartDistance)
	{
	// 어택 상태로 넘어가
		delyTime += GetWorld()->DeltaRealTimeSeconds * 10;
		UE_LOG(LogTemp, Error, TEXT("%f"), delyTime);
		delyTime = 0;
		mState = EEnmeyState::Attack;
		currentTIme = 0;
		PRINT_LOG(TEXT("Go to Attack!!!"));
	}
	// 다 아니라면 백스텝으로 가
	else
	{
		mState = EEnmeyState::Backstep;
		currentTIme = 0;
		Backspeed = 0;
		PRINT_LOG(TEXT("Go to Beckstep!!!"));

	}

}
// 플레이어와의 거리가 백스텝 거리보다 작으면 백스텝을 실행하고 아이들상태로 돌아가
void UOSY_PursuerFSM::BackstepState()
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

	currentTIme +=GetWorld()->DeltaRealTimeSeconds;
	float FastTime = 0.1f;
	float SlowTime = 0.8f;
	float EndTime = 1;
	// 만약 현재시간이 패스트타임보다 작거나같으면,
	if (currentTIme <= FastTime)
	{
	Backspeed = 500* sqrt(currentTIme/FastTime);
	FVector P = me->GetActorLocation()+Direction*-1* Backspeed *GetWorld()->DeltaRealTimeSeconds;
	me->SetActorLocation(P);
	}
	else if (currentTIme < SlowTime)
	{
	Backspeed = 500;
	FVector P = me->GetActorLocation() + Direction * -1 * Backspeed * GetWorld()->DeltaRealTimeSeconds;
	me->SetActorLocation(P);
	}
	else if (currentTIme < EndTime)
	{
	Backspeed = 500*(1-FMath::Pow(((currentTIme-SlowTime)/EndTime-SlowTime),2));
	}
	else
	{
	PRINT_LOG(TEXT("BE_Back to Idle"))
	mState = EEnmeyState::Idle;
	}
}


// 플레이어와의 거리가 무브거리 안쪽이면 걸어가다가 어택 거리가 되면 아이들상태로 돌아간다

// 필요속성 : 타겟, 이동속도
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

	// 만약 플레이어와의 거리가 러시스타트거리보다 크면 러쉬를 실행하고, 실행한 다음 아이들로 돌아가
	// 시간에 흐름에 따라 페스트타임일때는 살짝느리게,슬로우타임일때는 정속도,엔드타임일때는 다시 느리게 움직인다
	// 필요속성 : 현재시간, 페스트타임시간,슬로우타임시간, 엔드타임시간, 러시스피드
	if (distance > AttackStartDistance  )
	{
		PRINT_LOG(TEXT("RUSH!!!"));
		// 시간이 흘렀으니까
		currentTIme += GetWorld()->DeltaTimeSeconds;
		float FastTime = 0.1;
		float SlowTime = 0.8;
		float EndTime = 1;
		// 만약 현재시간이 페스트타임보다 작거나 같다면
		if (currentTIme <= FastTime)
		{
			// 러시스피드를 살짝 느리게 한다
			Rushspeed = 2500 * sqrt(currentTIme / FastTime);
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
		// 그렇지 않으면서 현재 시간이 슬로우타임보다 작다면 정속도로 움직인다.
		else if (currentTIme < SlowTime)
		{
			Rushspeed = 2500;
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
		// 그것도 아니고 엔드타임보다 작다면 다시 느리게 움직인다.
		else 
		{
			Rushspeed = 2500 * (1 - FMath::Pow(((currentTIme / SlowTime) - (currentTIme / SlowTime)), 2));
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
	}
	else
	{
		delyTime += GetWorld()->DeltaRealTimeSeconds * 1000;
		PRINT_LOG(TEXT("R_Back to Idle"));
		mState = EEnmeyState::Idle;
		delyTime = 0;
	}
}

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

	//만약 플레이어와의 거리가 러시거리보다 크면 무브를 하고 그렇지 않으면 아이들로 돌아가
	//1. 만약 플레이어와의 거리가 러시거리보다 크면
	
	if (distance > RushStartDistance)
	{
	//2. 무브를 실행하고
		FVector P = me->GetActorLocation() + Direction * Walkspeed * GetWorld()->DeltaRealTimeSeconds;
		UE_LOG(LogTemp, Error, TEXT("%f"),Walkspeed)
		UE_LOG(LogTemp, Error, TEXT("%f"),P.X)
		UE_LOG(LogTemp, Error, TEXT("%f"),P.Y)
		me->SetActorLocation(P);
		PRINT_LOG(TEXT("Walk!!!"));
	}
	//3. 그렇지 않다면
	else
	{
		//4. 아이들로 돌아가
		
		
		PRINT_LOG(TEXT("W_Back to Idle"));
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
	if (distance > BackstepStartDistance )
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
		// 몸에 달려있는 스켈레탈의 로테이션을 
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

		// 이쪽으로 상태가 넘어오면 어택1을 하고
		PRINT_LOG(TEXT("ATTACK1!!!!"));
// 		Direction.Z = 0;
// 		FVector forward = me->GetActorForwardVector();
// 		forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
// 		// Enemy forward 벡터가 direction 방향으로 일치시키고 싶다.
// 		me->SetActorRotation(forward.Rotation());

		//하고 싶은 일 : 어택상태에 들어가면 손에 있는 스켈레탈메쉬가 x 방향으로 90% 내려갔다 올라오게 하고 싶고 그 일정시간이 지난후 돌아가고 싶다
		// 스켈레탈메쉬가 x 방향으로 90% 내려갔다 올라오게 하고 싶다
		// 스켈레탈 메쉬가 뭔데?
		// compSword 임(전역에 선언했으니 그냥 써)
		// 얼마동안 내려가고 얼마동안 올라올거야?
		// 1초동안 내려가고 1초동안 올라올거야
		float AttackStarttoHalf = 1;
		float AttackHalftoEnd =1;
		// 1초 동안 어떻게 내려갈건데?
		// 
		// 1초동안 수업에서 배운 러프의 방법으로 내려갈거야
		// 그러면 1초동안 어떻게 내려갈건데?
		// 1초동안 수업에서 배운 러프의 방법으로 올라올거야
		// 그 일정시간이 지난후 돌아가고 싶다
		// 일정시간은 얼마나 되는데?
		// 한 2초정도 기다렸다가 돌아갈거야
		
		// 만약 어택거리 보다 크다면 아이들로 돌아가
		if (distance > AttackStartDistance)
		{
			PRINT_LOG(TEXT("A1_Back to Idle"));
			mState = EEnmeyState::Idle;
		}
		//  아직 어택거리면 어택베이직으로 돌아가
		else
		{
			PRINT_LOG(TEXT("A1_Back to Attack Basic"));
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
	PRINT_LOG(TEXT("ATTACK2!!!!"));

		// 만약 어택거리 보다 크다면 아이들로 돌아가
		if (distance > AttackStartDistance)
		{
			PRINT_LOG(TEXT("A2_Back to Idle"));
				mState = EEnmeyState::Idle;
		}
	//  아직 어택거리면 어택베이직으로 돌아가
		else
		{
			PRINT_LOG(TEXT("A2_Back to Attack Basic"));
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

void UOSY_PursuerFSM::LowerAndRaiseWeapon()
{
	//if (me->compSword)
	//{
	//	FVector RootBoneTransform = me->compSword->GetBoneTransform(0);
	//	FVector NewLocation = RootBoneTransform.GetLocation();
	//}
}
