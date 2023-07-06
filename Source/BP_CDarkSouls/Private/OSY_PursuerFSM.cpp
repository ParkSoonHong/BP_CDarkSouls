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
	// Attack SubMachin ����
}

void UOSY_PursuerFSM::IdleState()
{

	// �÷��̾���� �Ÿ��� Ž���ϴٰ� ��ũ��ŸƮ���� ũ�� ���÷� �Ѿ��, ���ý�Ÿ�庸�� ũ�� ������·� �Ѿ��, �齺�ܽ�Ÿ�庸�� ũ�� �������� �Ѿ�� ��� �ƴϸ� �齺����
	// �ʿ�Ӽ� : �÷��̾���� �Ÿ�, ��ũ��ŸƮ�Ÿ�, ���ý�ŸƮ�Ÿ�,�齺�ܽ�ŸƮ�Ÿ�
	// 
	// �÷��̾���� �Ÿ��� Ž���Ѵ�
	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

	
	// ���� �÷��̾���� �Ÿ��� ��ũ��ŸƮ���� ũ��
	if (distance > WalkStartDistance)
	{
		// ��û��·� �Ѿ��
		PRINT_LOG(TEXT("Go to Rush!!!"));
		mState = EEnmeyState::Rush;
	}
	// �׷��� �ʰ� ���ú��� ũ�� ��ũ�ΰ�
	else if (distance > AttackStartDistance)
	{
	// ������·� �Ѿ��
		mState = EEnmeyState::Walk;
		PRINT_LOG(TEXT("Go to Walk!!!"));
	}
	// �Ѵ� �ƴϰ� ���ðŸ����� ª������
	else if(distance > BackstepStartDistance)
	{
	// ���� ���·� �Ѿ
		mState = EEnmeyState::Attack;
		PRINT_LOG(TEXT("Go to Attack!!!"));
	}
	else
	{
		mState = EEnmeyState::Backstep;
		PRINT_LOG(TEXT("Go to Beckstep!!!"));

	}

}
// �÷��̾���� �Ÿ��� �齺�� �Ÿ����� ������ �齺���� �����ϰ� ���̵���·� ���ư�
void UOSY_PursuerFSM::BackstepState()
{
	PRINT_LOG(TEXT("B_Back to Idle!!!"));
	FVector P = me->GetActorLocation()+FVector::ForwardVector*-1* Walkspeed *GetWorld()->DeltaRealTimeSeconds;
	mState = EEnmeyState::Idle;
}


// �÷��̾���� �Ÿ��� ����Ÿ� �����̸� �ɾ�ٰ� ���� �Ÿ��� �Ǹ� ���̵���·� ���ư���

// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
void UOSY_PursuerFSM::WalkState()
{
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

	//���� �÷��̾���� �Ÿ��� ����Ÿ����� ũ�� �����ϰ�, �׷��� �ʴٸ� ���̵�� ���ư�
	//1. ���� �÷��̾���� �Ÿ��� ����Ÿ����� ũ��
	if (distance > AttackStartDistance)
	{
	//2. ���긦 �����ϰ�
		FVector P= me->GetActorLocation()+Direction* Walkspeed *GetWorld()->DeltaRealTimeSeconds;
		me->SetActorLocation(P);
		PRINT_LOG(TEXT("Walk!!!"));
	}
	//3. �׷��� �ʴٸ�
	else
	{
	//4. ���̵�� ���ư�
		PRINT_LOG(TEXT("M_Back to Idle!!!"));

	}
}

void UOSY_PursuerFSM::RushState()
{
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
	
	// ���� �÷��̾���� �Ÿ��� Rush�Ÿ����� ũ�� �뽬�� �����ϰ�,���̵�� ���ư�
	if (distance > WalkStartDistance)
	{
	// ��ø� �����ϴµ�
		FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
		me->SetActorLocation(P);
	PRINT_LOG(TEXT("Rush!!!"));

	}
	//3. �׷��� ������ ���̵�� ���ư�
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

// ���� �÷��̾���� �Ÿ��� ���ðŸ��� ª���� ������ ������, �׷��� ������ ���̵�� ���ư�
//1. ���� �÷��̾���� �Ÿ��� ��
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
/// Attack �Լ�
/// </summary>
void UOSY_PursuerFSM::AttackBasic()
{
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

	// ������ Ȯ���ε� 40���δ� ����1��, �״��� 30�� ����2��, ������ 30�� ����3���� ������Ʈ�� �̵��ϰ� �ʹ�.
	// �ʿ�Ӽ� : ������ Ȯ��, 
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

	// �������� ���°� �Ѿ���� ����1�� �ϰ�
		//PRINT_LOG(TEXT("ATTACK1!!!!"));

	// ���� ���ðŸ� ���� ũ�ٸ� ���̵�� ���ư�
		if (distance > AttackStartDistance)
		{
		//PRINT_LOG(TEXT("A1_Back to Idle"));
		 mState = EEnmeyState::Idle;
		}
	//  ���� ���ðŸ��� ���ú��������� ���ư�
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());


	// �������� ���°� �Ѿ���� ����1�� �ϰ�
	//PRINT_LOG(TEXT("ATTACK2!!!!"));

		// ���� ���ðŸ� ���� ũ�ٸ� ���̵�� ���ư�
		if (distance > AttackStartDistance)
		{
			//PRINT_LOG(TEXT("A2_Back to Idle"));
				mState = EEnmeyState::Idle;
		}
	//  ���� ���ðŸ��� ���ú��������� ���ư�
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

	// �������� ���°� �Ѿ���� ����1�� �ϰ�
	PRINT_LOG(TEXT("ATTACK3!!!!"));

		// ���� ���ðŸ� ���� ũ�ٸ� ���̵�� ���ư�
		if (distance > AttackStartDistance)
		{
			PRINT_LOG(TEXT("A3_Back to Idle"));
				mState = EEnmeyState::Idle;
		}
	//  ���� ���ðŸ��� ���ú��������� ���ư�
		else
		{
			PRINT_LOG(TEXT("A3_Back to Attack Basic"));
			mAttackState = EEnmeyAttackState::AttackBasic;

		}
}
