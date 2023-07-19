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
	case EEnmeyState::Attack:
		AttackState();
		break;
	case EEnmeyState::Rush:
		RushState();
		break;
	case EEnmeyState::RushAttack:
		RushAttackState();
		break;
	case EEnmeyState::Attack1_1:
		Attack1_1State();
		break;
	case EEnmeyState::Attack1_2:
		Attack1_2State();
		break;
	case EEnmeyState::Attack1_3:
		Attack1_3State();
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
// 아이들에 있다가 어택으로 넘어가
// 어택에서 만약 러쉬스타트보다 크면 러쉬로 넘어가
// 그렇지 않다면 어택을 실행해
void UOSY_PursuerFSM::IdleState()
{
	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	currentTIme+=me->GetWorld()->DeltaTimeSeconds;
	UE_LOG(LogTemp,Error,TEXT("NowIDLE"))

	// 만약 2초가 흘렀다면
	if (currentTIme > DelayTime)
	{
		// 만약 어택거리보다 크면 
		if (distance > AttackStartDistance)
		{
		//워크 스테이트로 가
			currentTIme=0;
			mState= EEnmeyState::Walk;
		}
		//그렇지 않고 백스텝보다 크면
		else if (distance > BackstepStartDistance)
		{
		// 어택스테이트로 가
			currentTIme = 0;
			mState= EEnmeyState::Attack;
		}
		// 모두 아니면
		else
		{
		// 백스텝 스테이트로 가
			currentTIme=0;
			mState= EEnmeyState::Backstep;
			anim->animState = mState;
		}
	}
	
}
// 플레이어와의 거리가 백스텝 거리보다 작으면 백스텝을 실행하고 아이들상태로 돌아가
void UOSY_PursuerFSM::WalkState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	UE_LOG(LogTemp, Error, TEXT("NowWalk"))
	
	// 여기로 오면 워크해
	FVector P = me->GetActorLocation() + Direction * Walkspeed * GetWorld()->DeltaRealTimeSeconds;
	me->SetActorLocation(P);
	if (distance < AttackStartDistance)
	{
	currentTIme=0;
	mState= EEnmeyState::Attack;
	}

	
}

void UOSY_PursuerFSM::AttackState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	UE_LOG(LogTemp, Error, TEXT("NowAttack"))
	currentTIme+=me->GetWorld()->DeltaTimeSeconds;
	if (currentTIme > RushDelayTime)
	{
		if (distance > RushStartDistance)
		{
			currentTIme=0;
			mState= EEnmeyState::Rush;
			anim->animState = mState;
		}
		// 그렇지 않으면 랜덤값을 찾아서 어택1을 할지 , 어택2로 갈지 결정하고 스테이트 바꿔
		else
		{
			int percent= FMath::RandRange(1,3);
			percent = 3;
			if (percent > 2)
			{
				// attack1로가
				anim->bAttack1_1Play= true;
				me->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState= EEnmeyState::Attack1_1;
				anim->animState = mState;
			}
			else if (percent > 1)
			{
				// attack2로가
				anim->bAttack2Play=true;
				me->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState= EEnmeyState::Attack2;
				anim->animState = mState;
			}
			else
			{
				// Idle로가
				mState=EEnmeyState::Idle;
				anim->animState = mState;
			}
		}
	}
}

void UOSY_PursuerFSM::RushState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	UE_LOG(LogTemp, Error, TEXT("NowRUSH"))
	
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
	if (distance < RushAttackDistance&& currentTIme>RushDelayTime)
	{
		me->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		mState = EEnmeyState::RushAttack;
		anim->bRushAttackPlay = true;
		anim->animState = mState;

	}
		
}

void UOSY_PursuerFSM::RushAttackState()
{
	UE_LOG(LogTemp, Error, TEXT("RushAttack"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	UE_LOG(LogTemp, Error, TEXT("NowRUSHATTACK"))
	// 러시어택을 플레이한다.
	
	// 러시어택이 끝나면 대기로 돌아간다.
	// 노티파이가 찍히면 스테이트를 바꿔라
	if (anim->bRushAttackPlay == false)
	{
	UE_LOG(LogTemp, Error, TEXT("Succes"))
		me->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EEnmeyState::Idle;
		anim->animState = mState;
	
	}
	
	
}

void UOSY_PursuerFSM::Attack1_1State()
{
	//UE_LOG(LogTemp, Error, TEXT("Attack1-1"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	anim->animState = mState;
	
	UE_LOG(LogTemp, Error, TEXT("NowAttack1_1"))
	// Attack1_2로 갈 확률
	if (anim->bAttack1_1Play == false)
	{
		int Attack1_2Random=FMath::RandRange(1,2);
		Attack1_2Random = 1;
		if (Attack1_2Random > 1)
		{
			me->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			UE_LOG(LogTemp, Error, TEXT("?????"))
			anim->bAttack1_2Play= true;
			mState=EEnmeyState::Attack1_2;
			anim->animState= mState;
		}
		else
		{
			// 끝날때;
			me->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mState=EEnmeyState::Idle;
			anim->animState = mState;
			
		}
	}
	


}

void UOSY_PursuerFSM::Attack1_2State()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	anim->animState = mState;
	UE_LOG(LogTemp, Error, TEXT("NowAttack1_2"))

	int Attack1_3Random = FMath::RandRange(1, 2);
	if (anim->bAttack1_2Play == false)
	{
		if (Attack1_3Random > 1)
		{
			me->compSword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			anim->bAttack1_3Play = true;
			mState = EEnmeyState::Attack1_3;
			anim->animState = mState;
		}
		else
		{
			me->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mState = EEnmeyState::Idle;
			anim->animState = mState;
			anim->bAttack1_1Play=true;
			anim->bAttack1_2Play = true;
		}
	}

}

void UOSY_PursuerFSM::Attack1_3State()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	anim->animState = mState;
	UE_LOG(LogTemp, Error, TEXT("NowAttack1_3"))

	if (anim->bAttack1_3Play == false)
	{
		me->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EEnmeyState::Idle;
		anim->animState = mState;
	}
}

void UOSY_PursuerFSM::Attack2State()
{
	UE_LOG(LogTemp, Error, TEXT("NowAttack_2"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	if (anim->bAttack2Play == false)
	{
		me->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EEnmeyState::Idle;
		anim->animState = mState;
	}
}

void UOSY_PursuerFSM::BackstepState()
{
	UE_LOG(LogTemp, Error, TEXT("NowBACKSTEP"))
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



	anim->animState = mState;
	
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


//-------------------------피격----------------------//
void UOSY_PursuerFSM::ReciveDamage(float value)
{
	if (HP - value > 0)
	{
		HP -= value;
		//UE_LOG(LogTemp, Log, TEXT("OLDDS HP=%d"), HP);
	}
	else
	{
		HP = 0;
		mState = EEnmeyState::Die;
	}
}


