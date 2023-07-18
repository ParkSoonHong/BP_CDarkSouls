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

	//UOSY_AnimInstance* �Ҵ�
	anim = Cast<UOSY_AnimInstance>(me->GetMesh()->GetAnimInstance());
	
	
}


// Called every frame
void UOSY_PursuerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());


	// FSM ����
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
// ���̵鿡 �ִٰ� �������� �Ѿ
// ���ÿ��� ���� ������ŸƮ���� ũ�� ������ �Ѿ
// �׷��� �ʴٸ� ������ ������
void UOSY_PursuerFSM::IdleState()
{
	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	currentTIme+=me->GetWorld()->DeltaTimeSeconds;
	UE_LOG(LogTemp,Error,TEXT("NowIDLE"))

	// ���� 2�ʰ� �귶�ٸ�
	if (currentTIme > DelayTime)
	{
		// ���� ���ðŸ����� ũ�� 
		if (distance > AttackStartDistance)
		{
		//��ũ ������Ʈ�� ��
			mState= EEnmeyState::Walk;
		}
		//�׷��� �ʰ� �齺�ܺ��� ũ��
		else if (distance > BackstepStartDistance)
		{
		// ���ý�����Ʈ�� ��
			mState= EEnmeyState::Attack;
		}
		// ��� �ƴϸ�
		else
		{
		// �齺�� ������Ʈ�� ��
			mState= EEnmeyState::Backstep;
		}
	}
	
}
// �÷��̾���� �Ÿ��� �齺�� �Ÿ����� ������ �齺���� �����ϰ� ���̵���·� ���ư�
void UOSY_PursuerFSM::WalkState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	UE_LOG(LogTemp, Error, TEXT("NowWalk"))
	
	// ����� ���� ��ũ��
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
		// �׷��� ������ �������� ã�Ƽ� ����1�� ���� , ����2�� ���� �����ϰ� ������Ʈ �ٲ�
		else
		{
			int percent= FMath::RandRange(1,5);
			if (percent > 3)
			{
				// attack1�ΰ�
				mState= EEnmeyState::Attack1;
				anim->animState = mState;
			}
			else if (percent > 1)
			{
				// attack2�ΰ�
				mState= EEnmeyState::Attack2;

				anim->animState = mState;
			}
			else
			{
				// Idle�ΰ�
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
	// ���� ����ð��� �佺ƮŸ�Ӻ��� �۰ų� ���ٸ�
	if (currentTIme <= FastTime)
	{
			// ���ý��ǵ带 ��¦ ������ �Ѵ�
			Rushspeed = 2500 * sqrt(currentTIme / FastTime);
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
	}
		// �׷��� �����鼭 ���� �ð��� ���ο�Ÿ�Ӻ��� �۴ٸ� ���ӵ��� �����δ�.
	else if (currentTIme < SlowTime)
	{
			Rushspeed = 2500;
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
	}
		// �װ͵� �ƴϰ� ����Ÿ�Ӻ��� �۴ٸ� �ٽ� ������ �����δ�.
	else
	{
			Rushspeed = 2500 * (1 - FMath::Pow(((currentTIme / SlowTime) - (currentTIme / SlowTime)), 2));
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
	}
	if (distance < RushAttackDistance&& currentTIme>RushDelayTime)
	{
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
	// ���þ����� �÷����Ѵ�.
	
	// ���þ����� ������ ���� ���ư���.
	// ��Ƽ���̰� ������ ������Ʈ�� �ٲ��
	if (anim->bRushAttackPlay == false)
	{
	UE_LOG(LogTemp, Error, TEXT("Succes"))
		
		mState = EEnmeyState::Idle;
		anim->animState = mState;
	
	}
	
	
}

void UOSY_PursuerFSM::Attack1State()
{
	//UE_LOG(LogTemp, Error, TEXT("Attack1-1"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();
	anim->animState = mState;
	
	UE_LOG(LogTemp, Error, TEXT("NowAttack_1"))
	//����11�� ������ ����ϰ�, ������ Ȯ���� 2�� ����ϰ�, �� ������ Ȯ���� 3�� ����Ѵ�.
	// �ʿ�Ӽ�, 2�� ����� Ȯ��, 3�� ����� Ȯ��
	int Attack12 = FMath::RandRange(1,5);
	int Attack13 = FMath::RandRange(1,5);

	if(Attack12>2)
	{
		//UE_LOG(LogTemp, Error, TEXT("Attack1-2"))
		
		if (Attack13 > 2)
		{
			//UE_LOG(LogTemp, Error, TEXT("Attack1-3"))
			
		}
		else
		{
			
		
			mState= EEnmeyState::Idle;
		}
			
			mState= EEnmeyState::Idle;
	}


	mState= EEnmeyState::Idle;

}

void UOSY_PursuerFSM::Attack2State()
{
	UE_LOG(LogTemp, Error, TEXT("NowAttack_2"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	mState = EEnmeyState::Idle;
}

void UOSY_PursuerFSM::BackstepState()
{
	//UE_LOG(LogTemp, Error, TEXT("BACKSTEP"))
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

	currentTIme +=GetWorld()->DeltaRealTimeSeconds;
	float FastTime = 0.1f;
	float SlowTime = 0.8f;
	float EndTime = 1;
	// ���� ����ð��� �н�ƮŸ�Ӻ��� �۰ų�������,
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


//-------------------------�ǰ�----------------------//
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


