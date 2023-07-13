// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_PursuerFSM.h"
#include "Templates/Casts.h"
#include "PSH_CPlayer.h"
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

	Target = Cast<APSH_CPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), APSH_CPlayer::StaticClass()));
	me = Cast<AOSY_Pursuer>(GetOwner());
	
	
}


// Called every frame
void UOSY_PursuerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

}

void UOSY_PursuerFSM::IdleState()
{

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
		mState = EEnmeyState::Walk;
	}
	// 그렇지 않고 어택거리보다 크면 러시로 가
	else if (distance > AttackStartDistance)
	{
	// 러시상태로 넘어가고
		currentTIme = 0;
		Rushspeed = 0;
		mState = EEnmeyState::Rush;
	}
	// 그렇지 않고 백스텝거리보다 크면 어택으로 가
	else if(distance > BackstepStartDistance)
	{
		UE_LOG(LogTemp, Log, TEXT("why"));
		bAttackSelect= false;
		mState = EEnmeyState::Attack;
	}
	// 다 아니라면 백스텝으로 가
	else
	{
		currentTIme = 0;
		Backspeed = 0;
		mState = EEnmeyState::Backstep;
	}
	UE_LOG(LogTemp, Log, TEXT("dis= %f, BackstepStartDistance= %f, AttackStartDistance= %f,RushStartDistance= %f"),distance,BackstepStartDistance,AttackStartDistance,RushStartDistance);

}
// 플레이어와의 거리가 백스텝 거리보다 작으면 백스텝을 실행하고 아이들상태로 돌아가
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
		me->SetActorLocation(P);
	}
	//3. 그렇지 않다면
	else
	{
		mState = EEnmeyState::Idle;
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

	// 만약 플레이어와의 거리가 러시스타트거리보다 크면 러쉬를 실행하고, 실행한 다음 아이들로 돌아가
	// 시간에 흐름에 따라 페스트타임일때는 살짝느리게,슬로우타임일때는 정속도,엔드타임일때는 다시 느리게 움직인다
	// 필요속성 : 현재시간, 페스트타임시간,슬로우타임시간, 엔드타임시간, 러시스피드
	if (distance > AttackStartDistance )
	{
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
		delayTime += GetWorld()->DeltaRealTimeSeconds * 1000;
		mState = EEnmeyState::Idle;
		delayTime = 0;
	}
}

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
	mState = EEnmeyState::Idle;
	}
}

void UOSY_PursuerFSM::DamageState()
{
	
}

void UOSY_PursuerFSM::DieState()
{
	GetOwner()->Destroy();
}

void UOSY_PursuerFSM::AttackState()
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

	//랜덤한 확률로 패턴1,2,3d
	if (!bAttackSelect) { pattern = FMath::RandRange(1, 3); bAttackSelect = true;}
	if (pattern > 2)
	{
	bAttackdirOk=false;
	LowerAndRaiseWeapon();
	}
	else if (pattern > 1)
	{
	bAttackdirOk = false;
	LowerAndRaiseWeapon2();
	}
	else
	{
	bAttackdirOk = false;
	LowerAndRaiseWeapon3();
	}
}


void UOSY_PursuerFSM::LowerAndRaiseWeapon()
{
	
	currentTIme += GetWorld()->DeltaTimeSeconds;
	if (currentTIme<1)
	{
		FVector P0=me->GetActorLocation();
		if (!bAttackdirOk)
		{
			Attackdir = me->GetActorRightVector();
			Attackdir.Z=0;
			bAttackdirOk=true;

		}
		Attackdir.Normalize();
		FVector vt=Attackdir*100*GetWorld()->DeltaTimeSeconds;
		FVector P=P0+vt;
		me->SetActorLocation(P);
	}
	// 2초동안 내렸다가
	else if (currentTIme < 2)
	{ // 1초동안은 내려가세요							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90-20*((currentTIme-1)/1), -90 * ((currentTIme-1) / 1)));
	}
	else if (currentTIme < 3)
	{ // 2~3초 동안 까지 올라가세요					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 70+20*((currentTIme-2)/1), -90 + (90 * ((currentTIme - 2) / 1))));
		// 1초동안 위로 올려
	}
	else
	{
		mState = EEnmeyState::Idle;
	}
	
}

void UOSY_PursuerFSM::LowerAndRaiseWeapon2()
{
	currentTIme += GetWorld()->DeltaTimeSeconds;
	if (currentTIme < 1)
	{
		FVector P0 = me->GetActorLocation();
		if (!bAttackdirOk)
		{
			Attackdir = me->GetActorRightVector() * -1;
			Attackdir.Z = 0;
			bAttackdirOk = true;

		}
		Attackdir.Normalize();
		FVector vt = Attackdir * 100 * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		me->SetActorLocation(P);
	}
	// 2초동안 내렸다가
	else if (currentTIme < 2)
	{ // 1초동안은 내려가세요							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90 - 20 * ((currentTIme - 1) / 1), -90 * ((currentTIme - 1) / 1)));
	}
	else if (currentTIme < 3)
	{ // 2~3초 동안 까지 올라가세요					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 70 + 20 * ((currentTIme - 2) / 1), -90 + (90 * ((currentTIme - 2) / 1))));
		// 1초동안 위로 올려
	}
	else
	{
		mState = EEnmeyState::Idle;
	}
}

void UOSY_PursuerFSM::LowerAndRaiseWeapon3()
{
	currentTIme += GetWorld()->DeltaTimeSeconds;
	if (currentTIme < 1)
	{
		
	}
	// 2초동안 내렸다가
	else if (currentTIme < 2)
	{ // 1초동안은 내려가세요							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90 - 20 * ((currentTIme - 1) / 1), -90 * ((currentTIme - 1) / 1)));
	}
	else if (currentTIme < 3)
	{ // 2~3초 동안 까지 올라가세요					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 70 + 20 * ((currentTIme - 2) / 1), -90 + (90 * ((currentTIme - 2) / 1))));
		// 1초동안 위로 올려
	}
	else
	{
		mState = EEnmeyState::Idle;
	}
}

//-------------------------피격----------------------//
void UOSY_PursuerFSM::ReciveDamage(float value)
{
	if (HP - value > 0)
	{
		HP -= value;
		UE_LOG(LogTemp, Log, TEXT("OLDDS HP=%d"), HP);
	}
	else
	{
		HP = 0;
		mState = EEnmeyState::Die;
	}
}


