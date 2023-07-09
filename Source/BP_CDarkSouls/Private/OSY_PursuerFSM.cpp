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

	FVector Direction = Target->GetActorLocation()-me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();


	PRINT_LOG(TEXT("IDLE!!!"))
	// ���� �÷��̾���� �Ÿ��� ���ý�ŸƮ���� ũ��
	if (distance > RushStartDistance)
	{
		// ��ũ ���·� �Ѿ��
		PRINT_LOG(TEXT("Go to Walk!!!"));
		mState = EEnmeyState::Walk;
	}
	else if (distance > AttackStartDistance)
	{
		// ���� ����ð��� ��ž �ð�ũ�� ���¸� �����ϰ�, �׷��� �ʴٸ� �ƹ��͵� ������
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
	// �� �ƴ϶�� �齺������ ��
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
// �÷��̾���� �Ÿ��� �齺�� �Ÿ����� ������ �齺���� �����ϰ� ���̵���·� ���ư�
void UOSY_PursuerFSM::WalkState()
{
	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	//���� �÷��̾���� �Ÿ��� ���ðŸ����� ũ�� ���긦 �ϰ� �׷��� ������ ���̵�� ���ư�
	//1. ���� �÷��̾���� �Ÿ��� ���ðŸ����� ũ��
	
	if (distance > RushStartDistance)
	{
	//2. ���긦 �����ϰ�
		FVector P = me->GetActorLocation() + Direction * Walkspeed * GetWorld()->DeltaRealTimeSeconds;
	
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

void UOSY_PursuerFSM::RushState()
{

	FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	currentTime += me->GetWorld()->DeltaTimeSeconds;
	if (distance > AttackStartDistance  )
	{
		PRINT_LOG(TEXT("RUSH!!!"));
		// �ð��� �귶���ϱ�
		
		float FastTime = 0.1;
		float SlowTime = 0.8;
		float EndTime = 1;
		// ���� ����ð��� �佺ƮŸ�Ӻ��� �۰ų� ���ٸ�
		if (currentTime <= FastTime)
		{
			// ���ý��ǵ带 ��¦ ������ �Ѵ�
			Rushspeed = 2500 * sqrt(currentTime / FastTime);
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
		// �׷��� �����鼭 ���� �ð��� ���ο�Ÿ�Ӻ��� �۴ٸ� ���ӵ��� �����δ�.
		else if (currentTime < SlowTime)
		{
			Rushspeed = 2500;
			FVector P = me->GetActorLocation() + Direction * Rushspeed * GetWorld()->DeltaRealTimeSeconds;
			me->SetActorLocation(P);
		}
		// �װ͵� �ƴϰ� ����Ÿ�Ӻ��� �۴ٸ� �ٽ� ������ �����δ�.
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
	// ���� ����ð��� �н�ƮŸ�Ӻ��� �۰ų�������,
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

		// �ִϸ��̼� ���� �� ���θ� ��Ÿ���� ������ �߰��մϴ�.
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
	{ // 1�ʵ����� ����������							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, -90 * (AttackTime / 1)));
		UE_LOG(LogTemp, Error, TEXT("LAR1D"))
		UE_LOG(LogTemp, Error, TEXT("CT1: %f"), AttackTime)
	}
	else if (AttackTime < 2)
	{ // 1~2�� ���� ���� �ö󰡼���					y	z		x
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
	{ // 1�ʵ����� ����������							y	z		x
		me->compSword->SetRelativeRotation(FRotator((90 * (currentTime / 1)), 90, 0));
	}
	else if (currentTime < 2)
	{ // 1~2�� ���� ���� �ö󰡼���					y									z		x
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
	{ // 1�ʵ����� ����������							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, 90 * (currentTime / 1)));
	}
	else if (currentTime < 2)
	{ // 1~2�� ���� ���� �ö󰡼���					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90, 90 + (90 * ((currentTime - 1) / 1))));
	}
	else
	{
		currentTime = 0;
//		mState = EEnmeyState::Idle;
	}

}

// �ָ��� �ϴ°��� ����ӽ� �ذ��ϱ� : ���������� ���ư��� �����̸� �ɾ �����ư��� �ϴ°Ŵ�/ �ȵƴ� ������ �׳� ���ٷ� ���ư������� �ڵ忴�� ����

// currentTime += me->GetWorld()->DeltaTimeSeconds;
//	UE_LOG(LogTemp, Error, TEXT("Current Time: %f"), currentTime);


//Ŀ��Ʈ�� �Ƚמ�������