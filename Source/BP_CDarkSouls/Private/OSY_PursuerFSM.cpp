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
	// Attack SubMachin ����
}

void UOSY_PursuerFSM::IdleState()
{

	// �÷��̾���� �Ÿ��� Ž���ϴٰ� ���ý�ŸƮ���� ũ�� ����� �Ѿ��, ���ý�Ÿ�庸�� ũ�� ���û��·� �Ѿ��, �齺�ܽ�Ÿ�庸�� ũ�� �������� �Ѿ�� ��� �ƴϸ� �齺����
	// �ʿ�Ӽ� : �÷��̾���� �Ÿ�, ���ý�ŸƮ�Ÿ�, ���ý�ŸƮ�Ÿ�,�齺�ܽ�ŸƮ�Ÿ�
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

	
	// ���� �÷��̾���� �Ÿ��� ���ý�ŸƮ���� ũ��
	if (distance > RushStartDistance)
	{
		// ��ũ ���·� �Ѿ��
		PRINT_LOG(TEXT("Go to Walk!!!"));
		mState = EEnmeyState::Walk;
	}
	// �׷��� �ʰ� ���ðŸ����� ũ�� ���÷� ��
	else if (distance > AttackStartDistance)
	{
	// ���û��·� �Ѿ��
		currentTIme = 0;
		Rushspeed = 0;
		delyTime += GetWorld()->DeltaRealTimeSeconds * 100;
		UE_LOG(LogTemp,Error,TEXT("%f"),delyTime);
		delyTime = 0;
		mState = EEnmeyState::Rush;
		PRINT_LOG(TEXT("Go to Rush!!!"));
	}
	// �׷��� �ʰ� �齺�ܰŸ����� ũ�� �������� ��
	else if(distance > BackstepStartDistance)
	{
	// ���� ���·� �Ѿ
		delyTime += GetWorld()->DeltaRealTimeSeconds * 10;
		UE_LOG(LogTemp, Error, TEXT("%f"), delyTime);
		delyTime = 0;
		mState = EEnmeyState::Attack;
		currentTIme = 0;
		PRINT_LOG(TEXT("Go to Attack!!!"));
	}
	// �� �ƴ϶�� �齺������ ��
	else
	{
		mState = EEnmeyState::Backstep;
		currentTIme = 0;
		Backspeed = 0;
		PRINT_LOG(TEXT("Go to Beckstep!!!"));

	}

}
// �÷��̾���� �Ÿ��� �齺�� �Ÿ����� ������ �齺���� �����ϰ� ���̵���·� ���ư�
void UOSY_PursuerFSM::BackstepState()
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
	PRINT_LOG(TEXT("BE_Back to Idle"))
	mState = EEnmeyState::Idle;
	}
}


// �÷��̾���� �Ÿ��� ����Ÿ� �����̸� �ɾ�ٰ� ���� �Ÿ��� �Ǹ� ���̵���·� ���ư���

// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
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

	// ���� �÷��̾���� �Ÿ��� ���ý�ŸƮ�Ÿ����� ũ�� ������ �����ϰ�, ������ ���� ���̵�� ���ư�
	// �ð��� �帧�� ���� �佺ƮŸ���϶��� ��¦������,���ο�Ÿ���϶��� ���ӵ�,����Ÿ���϶��� �ٽ� ������ �����δ�
	// �ʿ�Ӽ� : ����ð�, �佺ƮŸ�ӽð�,���ο�Ÿ�ӽð�, ����Ÿ�ӽð�, ���ý��ǵ�
	if (distance > AttackStartDistance  )
	{
		PRINT_LOG(TEXT("RUSH!!!"));
		// �ð��� �귶���ϱ�
		currentTIme += GetWorld()->DeltaTimeSeconds;
		float FastTime = 0.1;
		float SlowTime = 0.8;
		float EndTime = 1;
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);


	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

	//���� �÷��̾���� �Ÿ��� ���ðŸ����� ũ�� ���긦 �ϰ� �׷��� ������ ���̵�� ���ư�
	//1. ���� �÷��̾���� �Ÿ��� ���ðŸ����� ũ��
	
	if (distance > RushStartDistance)
	{
	//2. ���긦 �����ϰ�
		FVector P = me->GetActorLocation() + Direction * Walkspeed * GetWorld()->DeltaRealTimeSeconds;
		UE_LOG(LogTemp, Error, TEXT("%f"),Walkspeed)
		UE_LOG(LogTemp, Error, TEXT("%f"),P.X)
		UE_LOG(LogTemp, Error, TEXT("%f"),P.Y)
		me->SetActorLocation(P);
		PRINT_LOG(TEXT("Walk!!!"));
	}
	//3. �׷��� �ʴٸ�
	else
	{
		//4. ���̵�� ���ư�
		
		
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

// ���� �÷��̾���� �Ÿ��� ���ðŸ��� ª���� ������ ������, �׷��� ������ ���̵�� ���ư�
//1. ���� �÷��̾���� �Ÿ��� ��
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
		// ���� �޷��ִ� ���̷�Ż�� �����̼��� 
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

		// �������� ���°� �Ѿ���� ����1�� �ϰ�
		PRINT_LOG(TEXT("ATTACK1!!!!"));
// 		Direction.Z = 0;
// 		FVector forward = me->GetActorForwardVector();
// 		forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
// 		// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
// 		me->SetActorRotation(forward.Rotation());

		//�ϰ� ���� �� : ���û��¿� ���� �տ� �ִ� ���̷�Ż�޽��� x �������� 90% �������� �ö���� �ϰ� �Ͱ� �� �����ð��� ������ ���ư��� �ʹ�
		// ���̷�Ż�޽��� x �������� 90% �������� �ö���� �ϰ� �ʹ�
		// ���̷�Ż �޽��� ����?
		// compSword ��(������ ���������� �׳� ��)
		// �󸶵��� �������� �󸶵��� �ö�ðž�?
		// 1�ʵ��� �������� 1�ʵ��� �ö�ðž�
		float AttackStarttoHalf = 1;
		float AttackHalftoEnd =1;
		// 1�� ���� ��� �������ǵ�?
		// 
		// 1�ʵ��� �������� ��� ������ ������� �������ž�
		// �׷��� 1�ʵ��� ��� �������ǵ�?
		// 1�ʵ��� �������� ��� ������ ������� �ö�ðž�
		// �� �����ð��� ������ ���ư��� �ʹ�
		// �����ð��� �󸶳� �Ǵµ�?
		// �� 2������ ��ٷȴٰ� ���ư��ž�
		
		// ���� ���ðŸ� ���� ũ�ٸ� ���̵�� ���ư�
		if (distance > AttackStartDistance)
		{
			PRINT_LOG(TEXT("A1_Back to Idle"));
			mState = EEnmeyState::Idle;
		}
		//  ���� ���ðŸ��� ���ú��������� ���ư�
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());


	// �������� ���°� �Ѿ���� ����1�� �ϰ�
	PRINT_LOG(TEXT("ATTACK2!!!!"));

		// ���� ���ðŸ� ���� ũ�ٸ� ���̵�� ���ư�
		if (distance > AttackStartDistance)
		{
			PRINT_LOG(TEXT("A2_Back to Idle"));
				mState = EEnmeyState::Idle;
		}
	//  ���� ���ðŸ��� ���ú��������� ���ư�
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

void UOSY_PursuerFSM::LowerAndRaiseWeapon()
{
	//if (me->compSword)
	//{
	//	FVector RootBoneTransform = me->compSword->GetBoneTransform(0);
	//	FVector NewLocation = RootBoneTransform.GetLocation();
	//}
}
