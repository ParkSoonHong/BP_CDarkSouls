// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_PursuerFSM.h"
#include "Templates/Casts.h"
#include "PSH_CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "OSY_Pursuer.h"
#include "../BP_CDarkSouls.h"
#include "Chaos/Vector.h"
#include "OSY_AnimInstance.h"

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

	//UOSY_AnimInstance* 할당
	anim = Cast<UOSY_AnimInstance>(me->GetMesh()->GetAnimInstance());
	
	
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
	case EEnmeyState::Walk:
		WalkState();
		break;
	case EEnmeyState::Rush:
		RushState();
		break;
	case EEnmeyState::RushAttack:
		RushAttackState();
		break;
	case EEnmeyState::Attack1:
		Attack1State();
		break;
	case EEnmeyState::Attack2:
		Attack2State();
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
	UE_LOG(LogTemp,Error,TEXT("IDLE"))

	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	if (distance > RushStartDistance)
	{
		mState = EEnmeyState::Walk;
		anim->animState=mState;
	}
	// 그렇지 않고 어택거리보다 크면 러시로 가
	else if (distance > AttackStartDistance)
	{
	// 러시상태로 넘어가고
		anim->bRushPlay= false;
		anim->bRushAttackPlay=false;
		currentTIme = 0;
		Rushspeed = 0;
		mState = EEnmeyState::Rush;
		anim->animState=mState;
	}
	// 그렇지 않고 백스텝거리보다 크면 어택으로 가
	else if(distance > BackstepStartDistance)
	{
		int Random = FMath::RandRange(1,2);
			if (Random == 1)
			{
				mState = EEnmeyState::Attack1;
				anim->animState=mState;
			}
			else
			{
				mState = EEnmeyState::Attack2;
				anim->animState = mState;
			}
	}
	// 다 아니라면 백스텝으로 가
	else
	{
		currentTIme = 0;
		Backspeed = 0;
		mState = EEnmeyState::Backstep;
		anim->animState=mState;
	}
}
// 플레이어와의 거리가 백스텝 거리보다 작으면 백스텝을 실행하고 아이들상태로 돌아가
void UOSY_PursuerFSM::WalkState()
{
	UE_LOG(LogTemp, Error, TEXT("WALK"))
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
	}
	//3. 그렇지 않다면
	else
	{
		mState = EEnmeyState::Idle;
		anim->animState = mState;

	}
}

void UOSY_PursuerFSM::RushState()
{
	UE_LOG(LogTemp, Error, TEXT("RUSH"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();


	if (distance > AttackStartDistance)
	{
		anim->bRushPlay = true;
		currentTIme += GetWorld()->DeltaTimeSeconds;
		float FastTime = 0.1;
		float SlowTime = 0.8;
		float EndTime = 2;
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
		anim->bRushPlay=false;
		UE_LOG(LogTemp, Error, TEXT("RushAttack"))
		anim->bRushAttackPlay = true;
		currentTIme = 0;
		//만약 러시어택이 끝났다면 아이들로 돌아가
		if (anim->bRushAttackPlay)
		{
		UE_LOG(LogTemp, Error, TEXT("True"))
		}
		else
		{
		UE_LOG(LogTemp, Error, TEXT("False"))
		}
		if (anim->bRushAttackPlay==false)
		{
		mState = EEnmeyState::Idle;
		delayTime = 0;
		anim->animState = mState;
		}
	}
}

void UOSY_PursuerFSM::RushAttackState()
{
	UE_LOG(LogTemp, Error, TEXT("RushAttack"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	anim->bRushPlay= false;
	if (distance > RushAttackDistance)
	{
	UE_LOG(LogTemp, Error, TEXT("RushAttackOK"))

	anim->bRushAttackPlay= true;
	}
	else
	{
		mState=EEnmeyState::Idle;
		anim->animState = mState;	
	}
	anim->animState = mState;
	
}

void UOSY_PursuerFSM::Attack1State()
{
	UE_LOG(LogTemp, Error, TEXT("Attack1-1"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	anim->animState = mState;
	
	anim->bAttack11Play= true;
	
	//어택11은 무조건 재생하고, 랜덤한 확률로 2를 재생하고, 또 랜덤한 확률로 3을 재생한다.
	// 필요속성, 2를 재생할 확률, 3을 재생할 확률
	int Attack12 = FMath::RandRange(1,5);
	int Attack13 = FMath::RandRange(1,5);

	if(Attack12>2)
	{
		UE_LOG(LogTemp, Error, TEXT("Attack1-2"))
		anim->bAttack11Play= false;
		anim->bAttack12Play= true;
		anim->bAttack13Play = false;
		if (Attack13 > 2)
		{
			UE_LOG(LogTemp, Error, TEXT("Attack1-3"))
			anim->bAttack11Play = false;
			anim->bAttack12Play = false;
			anim->bAttack13Play= true;
		}
		else
		{
			anim->bAttack11Play = false;
			anim->bAttack12Play = false;
			anim->bAttack13Play = false;
		
			mState= EEnmeyState::Idle;
		}
			anim->bAttack11Play = false;
			anim->bAttack12Play = false;
			anim->bAttack13Play = false;
			mState= EEnmeyState::Idle;
	}


	mState= EEnmeyState::Idle;

}

void UOSY_PursuerFSM::Attack2State()
{
	UE_LOG(LogTemp, Error, TEXT("Attack2"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	mState = EEnmeyState::Idle;
}

void UOSY_PursuerFSM::BackstepState()
{
	UE_LOG(LogTemp, Error, TEXT("BACKSTEP"))
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
	anim->animState = mState;
	}
}

void UOSY_PursuerFSM::DamageState()
{
	
}

void UOSY_PursuerFSM::DieState()
{
	GetOwner()->Destroy();
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


