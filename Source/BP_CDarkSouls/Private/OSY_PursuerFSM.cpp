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

	currentTime+= me->GetWorld()->DeltaRealTimeSeconds;

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

	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();


	PRINT_LOG(TEXT("IDLE!!!"))
	// 만약 플레이어와의 거리가 러시스타트보다 크면
	if (distance > RushStartDistance)
	{
		// 워크 상태로 넘어가고
		PRINT_LOG(TEXT("Go to Walk!!!"));
		mState = EEnmeyState::Walk;
	}
	else if (distance > AttackStartDistance)
	{
		// 만약 현재시간이 스탑 시간크면 상태를 변경하고, 그렇지 않다면 아무것도 하지마
		if (currentTime > stopTime)
		{
			currentTime = 0;
			Rushspeed = 0;
			mState = EEnmeyState::Rush;
			PRINT_LOG(TEXT("Go to Rush!!!"));
		}
	}
	else if(distance > BackstepStartDistance)
	{
	
		if (currentTime > stopTime)
		{
			mState = EEnmeyState::Attack;
			currentTime = 0;
			PRINT_LOG(TEXT("Go to Attack!!!"));
		}
		
	}
	// 다 아니라면 백스텝으로 가
	else
	{
		if (currentTime > stopTime)
		{
			currentTime = 0;
			Backspeed = 0;
			mState = EEnmeyState::Backstep;
			PRINT_LOG(TEXT("Go to Beckstep!!!"));
		}
	}

}
// 플레이어와의 거리가 백스텝 거리보다 작으면 백스텝을 실행하고 아이들상태로 돌아가
void UOSY_PursuerFSM::WalkState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	//만약 플레이어와의 거리가 러시거리보다 크면 무브를 하고 그렇지 않으면 아이들로 돌아가
	//1. 만약 플레이어와의 거리가 러시거리보다 크면
	
	if (distance > RushStartDistance)
	{
	//2. 무브를 실행하고
		FVector P = me->GetActorLocation() + Direction * Walkspeed * GetWorld()->DeltaRealTimeSeconds;
	
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

void UOSY_PursuerFSM::RushState()
{

	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	currentTime += me->GetWorld()->DeltaTimeSeconds;
	if (distance > AttackStartDistance  )
	{
		PRINT_LOG(TEXT("RUSH!!!"));
		// 시간이 흘렀으니까
		
		float FastTime = 0.1;
		float SlowTime = 0.8;
		float EndTime = 1;
		// 만약 현재시간이 페스트타임보다 작거나 같다면
		if (currentTime <= FastTime)
		{
			// 러시스피드를 살짝 느리게 한다
			Rushspeed = 2500 * sqrt(currentTime / FastTime);
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
		// 그렇지 않으면서 현재 시간이 슬로우타임보다 작다면 정속도로 움직인다.
		else if (currentTime < SlowTime)
		{
			Rushspeed = 2500;
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
		// 그것도 아니고 엔드타임보다 작다면 다시 느리게 움직인다.
		else 
		{
			Rushspeed = 2500 * (1 - FMath::Pow(((currentTime / SlowTime) - (currentTime / SlowTime)), 2));
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
	}
	else
	{
		PRINT_LOG(TEXT("R_Back to Idle"));
		mState = EEnmeyState::Idle;
		currentTime=0;
	}
}

void UOSY_PursuerFSM::BackstepState()
{

	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	currentTime += me->GetWorld()->DeltaTimeSeconds;
	float FastTime = 0.1f;
	float SlowTime = 0.8f;
	float EndTime = 1;
	// 만약 현재시간이 패스트타임보다 작거나같으면,
	if (currentTime <= FastTime)
	{
	Backspeed = 500* sqrt(currentTime/FastTime);
	FVector P = me->GetActorLocation()+Direction*-1* Backspeed *GetWorld()->DeltaRealTimeSeconds;
	me->SetActorLocation(P);
	}
	else if (currentTime < SlowTime)
	{
	Backspeed = 500;
	FVector P = me->GetActorLocation() + Direction * -1 * Backspeed * GetWorld()->DeltaRealTimeSeconds;
	me->SetActorLocation(P);
	}
	else if (currentTime < EndTime)
	{
	Backspeed = 500*(1-FMath::Pow(((currentTime-SlowTime)/EndTime-SlowTime),2));
	}
	else
	{
	PRINT_LOG(TEXT("BE_Back to Idle"))
	mState = EEnmeyState::Idle;
	currentTime =0;
	}
}

void UOSY_PursuerFSM::DamageState()
{
	
}

void UOSY_PursuerFSM::DieState()
{

}

void UOSY_PursuerFSM::AttackState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	if (currentTime > 2)
	{
		PRINT_LOG(TEXT("ATTACK!!!"));
		float AttackRandom = FMath::RandRange(1, 100);
		PRINT_LOG(TEXT("%f"), AttackRandom);
		
		if (distance > BackstepStartDistance)
		{
			if (AttackRandom > 60)
			{
				PRINT_LOG(TEXT("Do Attack1!!!"));
				LowerAndRaiseWeapon1();
			}
			else if (AttackRandom > 30)
			{
				PRINT_LOG(TEXT("Do Attack2!!!"));
				LowerAndRaiseWeapon2();
			}
			else
			{
				PRINT_LOG(TEXT("Do Attack3!!!"));
				LowerAndRaiseWeapon3();
			}
			
		}

		// 애니메이션 실행 중 여부를 나타내는 변수를 추가합니다.
	}

}

// void UOSY_PursuerFSM::AttackState()
// {
// 	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
// 	float distance = Direction.Length();
// 	Direction.Normalize();
// 
// 	currentTime+= me->GetWorld()->DeltaTimeSeconds;
// 
// 	if (currentTime > 2)
// 	{
// 		PRINT_LOG(TEXT("ATTACK!!!"));
// 		float AttackRandom = FMath::RandRange(1, 100);
// 		PRINT_LOG(TEXT("%f"), AttackRandom);
// 
// 		if (distance > BackstepStartDistance )
// 		{
// 			if (AttackRandom > 60)
// 			{
// 				PRINT_LOG(TEXT("Do Attack1!!!"));
// 				currentTime = 0;
// 				LowerAndRaiseWeapon1();
// 			}
// 			else if (AttackRandom > 30)
// 			{
// 				PRINT_LOG(TEXT("Do Attack2!!!"));
// 				currentTime = 0;
// 				LowerAndRaiseWeapon2();
// 			}
// 			else
// 			{
// 				PRINT_LOG(TEXT("Do Attack3!!!"));
// 				currentTime = 0;
// 				LowerAndRaiseWeapon3();
// 			}
// 
// 		}	
// 	}
// 
// }
//--------------------------------------------------------------------------//

void UOSY_PursuerFSM::LowerAndRaiseWeapon1()
{
	UE_LOG(LogTemp, Error, TEXT("In LAR1"))
	UE_LOG(LogTemp, Error, TEXT("AT1: %f"),AttackTime)
	
	if (AttackTime < 1)
	{ // 1초동안은 내려가세요							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, -90 * (AttackTime / 1)));
		UE_LOG(LogTemp, Error, TEXT("LAR1D"))
		UE_LOG(LogTemp, Error, TEXT("CT1: %f"), AttackTime)
	}
	else if (AttackTime < 2)
	{ // 1~2초 동안 까지 올라가세요					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, -90 + (90 * ((AttackTime - 1) / 1))));
		UE_LOG(LogTemp, Error, TEXT("LAR1U"))
		UE_LOG(LogTemp, Error, TEXT("CT1: %f"), AttackTime)
	}
	else
	{
		AttackTime = 0;
		bIsAnimating = false;
	}
	
}
	
void UOSY_PursuerFSM::LowerAndRaiseWeapon2()
{
	currentTime += me->GetWorld()->DeltaTimeSeconds;

	if (currentTime < 1)
	{ // 1초동안은 내려가세요							y	z		x
		me->compSword->SetRelativeRotation(FRotator((90 * (currentTime / 1)), 90, 0));
	}
	else if (currentTime < 2)
	{ // 1~2초 동안 까지 올라가세요					y									z		x
		me->compSword->SetRelativeRotation(FRotator((90 - +(90 * ((currentTime - 1) / 1)), 90, 0)));
	}
	else
	{
		currentTime = 0;
//		mState = EEnmeyState::Idle;
	}
}

void UOSY_PursuerFSM::LowerAndRaiseWeapon3()
{
	currentTime += me->GetWorld()->DeltaTimeSeconds;

	if (currentTime < 1)
	{ // 1초동안은 내려가세요							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, 90 * (currentTime / 1)));
	}
	else if (currentTime < 2)
	{ // 1~2초 동안 까지 올라가세요					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, 90 + (90 * ((currentTime - 1) / 1))));
	}
	else
	{
		currentTime = 0;
//		mState = EEnmeyState::Idle;
	}

}

// 주말에 하는것은 서브머신 해결하기 : 베이직으로 돌아갈때 딜레이를 걸어서 못돌아가게 하는거다/ 안됐던 이유는 그냥 막바로 돌아가버리는 코드였기 때문

// currentTime += me->GetWorld()->DeltaTimeSeconds;
//	UE_LOG(LogTemp, Error, TEXT("Current Time: %f"), currentTime);


//커랜트가 안쌓옄ㅋㅋㅋ