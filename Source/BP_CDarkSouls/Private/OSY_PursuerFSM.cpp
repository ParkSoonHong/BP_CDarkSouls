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

}

void UOSY_PursuerFSM::IdleState()
{

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
		mState = EEnmeyState::Walk;
	}
	// �׷��� �ʰ� ���ðŸ����� ũ�� ���÷� ��
	else if (distance > AttackStartDistance)
	{
	// ���û��·� �Ѿ��
		currentTIme = 0;
		Rushspeed = 0;
		mState = EEnmeyState::Rush;
	}
	// �׷��� �ʰ� �齺�ܰŸ����� ũ�� �������� ��
	else if(distance > BackstepStartDistance)
	{
		UE_LOG(LogTemp, Log, TEXT("why"));
		bAttackSelect= false;
		mState = EEnmeyState::Attack;
	}
	// �� �ƴ϶�� �齺������ ��
	else
	{
		currentTIme = 0;
		Backspeed = 0;
		mState = EEnmeyState::Backstep;
	}
	UE_LOG(LogTemp, Log, TEXT("dis= %f, BackstepStartDistance= %f, AttackStartDistance= %f,RushStartDistance= %f"),distance,BackstepStartDistance,AttackStartDistance,RushStartDistance);

}
// �÷��̾���� �Ÿ��� �齺�� �Ÿ����� ������ �齺���� �����ϰ� ���̵���·� ���ư�
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
		me->SetActorLocation(P);
	}
	//3. �׷��� �ʴٸ�
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
	if (distance > AttackStartDistance )
	{
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

	// Ÿ�ٹ������� ȸ���ϱ�
	// forward vector�� direction�� �ǰ� �ʹ�
	Direction.Z = 0;
	FVector forward = me->GetActorForwardVector();
	forward = FMath::Lerp(forward, Direction, 5 * GetWorld()->DeltaTimeSeconds);
	// Enemy forward ���Ͱ� direction �������� ��ġ��Ű�� �ʹ�.
	me->SetActorRotation(forward.Rotation());

	//������ Ȯ���� ����1,2,3d
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
	// 2�ʵ��� ���ȴٰ�
	else if (currentTIme < 2)
	{ // 1�ʵ����� ����������							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90-20*((currentTIme-1)/1), -90 * ((currentTIme-1) / 1)));
	}
	else if (currentTIme < 3)
	{ // 2~3�� ���� ���� �ö󰡼���					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 70+20*((currentTIme-2)/1), -90 + (90 * ((currentTIme - 2) / 1))));
		// 1�ʵ��� ���� �÷�
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
	// 2�ʵ��� ���ȴٰ�
	else if (currentTIme < 2)
	{ // 1�ʵ����� ����������							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90 - 20 * ((currentTIme - 1) / 1), -90 * ((currentTIme - 1) / 1)));
	}
	else if (currentTIme < 3)
	{ // 2~3�� ���� ���� �ö󰡼���					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 70 + 20 * ((currentTIme - 2) / 1), -90 + (90 * ((currentTIme - 2) / 1))));
		// 1�ʵ��� ���� �÷�
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
	// 2�ʵ��� ���ȴٰ�
	else if (currentTIme < 2)
	{ // 1�ʵ����� ����������							y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 90 - 20 * ((currentTIme - 1) / 1), -90 * ((currentTIme - 1) / 1)));
	}
	else if (currentTIme < 3)
	{ // 2~3�� ���� ���� �ö󰡼���					y	z		x
		me->compSword->SetRelativeRotation(FRotator(0, 70 + 20 * ((currentTIme - 2) / 1), -90 + (90 * ((currentTIme - 2) / 1))));
		// 1�ʵ��� ���� �÷�
	}
	else
	{
		mState = EEnmeyState::Idle;
	}
}

//-------------------------�ǰ�----------------------//
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


