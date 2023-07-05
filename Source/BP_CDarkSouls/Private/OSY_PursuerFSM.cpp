// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_PursuerFSM.h"
#include "Templates/Casts.h"
#include "UPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "OSY_Pursuer.h"
#include "../BP_CDarkSouls.h"

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

	// ...

	// FSM ����
	switch (mState)
	{
	case EEnmeyState::Idle:
		IdleState();
		break;
	case EEnmeyState::Move:
		MoveState();
		break;
	case EEnmeyState::Dash:
		DashState();
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

	// �÷��̾���� �Ÿ��� Ž���ϴٰ� ��ðŸ����Ǹ� ��û��·� �Ѿ��, ��ðŸ����� ª���� ������·� �Ѿ��, ���ðŸ��� �Ǹ� �������� �Ѿ��
	// �ʿ�Ӽ� : �÷��̾���� �Ÿ�, ��ðŸ�, ����Ÿ�,���ðŸ�
	// 
	// �÷��̾���� �Ÿ��� Ž���Ѵ�
	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	// ���� �÷��̾���� �Ÿ��� ��ðŸ����� �ִٸ�
	if (distance < DashDistance)
	{
		// ��û��·� �Ѿ��
		mState = EEnmeyState::Dash;
		PRINT_LOG(TEXT("DASH!!!"));
	}
	// �׷��� �ʰ� ��ðŸ����� ª�� ���ðŸ����� �� ����Ÿ��� �Ǹ�
	else if (distance < MoveDistance)
	{
	// ������·� �Ѿ��
		mState = EEnmeyState::Move;
		PRINT_LOG(TEXT("MOVE!!!"));
	}
	// �Ѵ� �ƴϰ� ���ðŸ����� ª������
	else
	{
	// ���� ���·� �Ѿ
		mState = EEnmeyState::Attack;
		PRINT_LOG(TEXT("Attack!!!"));

	}
	

}
// Ÿ�������� �̵��ϰ� �ʹ�.
// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
void UOSY_PursuerFSM::MoveState()
{
// 	�Ÿ��� Ž���ϰ� �ִ�
		FVector Direction = Target->GetActorLocation() - me->GetActorLocation();
		//float distance = Direction.Length();
		Direction.Normalize();
	
		FVector P = me->GetActorLocation() + Direction * speed * GetWorld()->DeltaTimeSeconds;
		me->SetActorLocation(P);
	
		Direction.Z = 0;
		// Ÿ�ٹ������� ȸ���ϱ�
		// forward vector�� direction�� �ǰ� �ʹ�
		FVector forward = me->GetActorForwardVector();
		forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
		// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
		me->SetActorRotation(forward.Rotation());

}

void UOSY_PursuerFSM::DashState()
{

}

void UOSY_PursuerFSM::AttackState()
{

}

void UOSY_PursuerFSM::DamageState()
{

}

void UOSY_PursuerFSM::DieState()
{

}

