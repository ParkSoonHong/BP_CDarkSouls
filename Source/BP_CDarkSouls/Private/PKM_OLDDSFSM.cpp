// Fill out your copyright notice in the Description page of Project Settings.


#include "PKM_OLDDSFSM.h"
#include "UPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "PKM_OLDDS.h"

// Sets default values for this component's properties
UPKM_OLDDSFSM::UPKM_OLDDSFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPKM_OLDDSFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Target = Cast<AUPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AUPlayer::StaticClass()));
	Me = Cast<APKM_OLDDS>(GetOwner());
}


// Called every frame
void UPKM_OLDDSFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	/*if (bDebugRange)
	{

	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), BackRange, 100, FColor::Black, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), attackRange, 100, FColor::Red, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), MoveRange, 100, FColor::Yellow, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), RunRange, 100, FColor::Blue, false, -1, 0, 2);

	}*/
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::BackStep:
		BackStepState();
		break;
	case EEnemyState::Run:
		RunState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	case  EEnemyState::RushAttack:
		RushAttackState();
		break;
	default:
		break;
	}
	// ...
}


void UPKM_OLDDSFSM::IdleState()
{
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
	}
	if (distance<BackRange) {
		if (FVector::DotProduct(direction,Me->GetActorForwardVector())>=0) {
			UE_LOG(LogTemp, Log, TEXT("Go BackStep"));
			currentTime = 0;
			BackStepSpeed = 0;
			mState = EEnemyState::BackStep;
			UE_LOG(LogTemp, Log, TEXT("memset Time=%f"), currentTime);
		}
		else {
			/*FVector Forward = Me->GetActorForwardVector();
			Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
			GetOwner()->SetActorRotation(Forward.Rotation())*/;
			UE_LOG(LogTemp, Log, TEXT("Turn.."));
			mState = EEnemyState::Idle;
		}
	}
	else if (distance<attackRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchAttack"));
		currentTime = 0;
		mState = EEnemyState::Attack;
	}
	else if (distance < MoveRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchMove"));
		mState = EEnemyState::Move;
	}
	else if (distance < RunRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchRun"));
		mState = EEnemyState::Run;
	}

}

void UPKM_OLDDSFSM::RunState()
{
	//direction = FVector(0, 0, 0);
	bRunAnimCheck = true;
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
	}
	if (distance < BackRange) {
		if (FVector::DotProduct(direction, Me->GetActorForwardVector())) {
			bRushAnimCheck = false;
			mState = EEnemyState::BackStep;
			UE_LOG(LogTemp, Log, TEXT("Go BackStep"));
		}
		else {
			FVector Forward = Me->GetActorForwardVector();
			Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
			GetOwner()->SetActorRotation(Forward.Rotation());
			UE_LOG(LogTemp, Log, TEXT("Turn.."));
		}
	}
	else if (distance < attackRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchAttack"));
		currentTime = 0;
		bRushAnimCheck = false;
		mState = EEnemyState::Attack;
	}
	else if (distance < MoveRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchMove"));
		bRushAnimCheck = false;
		mState = EEnemyState::Move;
	}
	else if (distance < RunRange)
	{
		Moving(500, direction);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("switchIdle"));
		bRunAnimCheck = false;
		mState = EEnemyState::Idle;
	}

}

void UPKM_OLDDSFSM::MoveState()
{
	//direction = FVector(0, 0, 0);
	/*if (currentTime<3)
	{
		currentTime += GetWorld()->DeltaTimeSeconds;
	}*/
	bWalkAnimCheck = true;
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
	}
	if (distance < BackRange) {
		if (FVector::DotProduct(direction, Me->GetActorForwardVector()) >= 0) {
			bWalkAnimCheck = false;
			mState = EEnemyState::BackStep;
			currentTime = 0;
			UE_LOG(LogTemp, Log, TEXT("Go BFackStep"));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("Turn.."));
			FVector Forward = Me->GetActorForwardVector();
			Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
			GetOwner()->SetActorRotation(Forward.Rotation());
		}
	}
	else if (distance < attackRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchAttack"));
		currentTime = 0;
		bWalkAnimCheck = false;
		mState = EEnemyState::Attack;
	}
	else if (distance < MoveRange)
	{
		if (MovingSpeed>200)
		{
			MovingSpeed = 200;
		}
		Moving(200, direction);
	}
	else if (distance < RunRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchRun"));
		bWalkAnimCheck = false;
		mState = EEnemyState::Run;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("switchIdle"));
		bWalkAnimCheck = false;
		mState = EEnemyState::Idle;
	}
	/*else {
		direction.Normalize();
		FVector P0 = Me->GetActorLocation();
		FVector vt = direction * 200 * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
		GetOwner()->SetActorRotation(Forward.Rotation());
		GetOwner()->SetActorLocation(P);
	}*/
	
}

void UPKM_OLDDSFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	float FixRotTime = 1;

	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
		
	}
	if (currentTime<FixRotTime)
	{
		FVector Forward = Me->GetActorForwardVector();
		Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
		GetOwner()->SetActorRotation(Forward.Rotation());
		//UE_LOG(LogTemp, Log, TEXT("fixRot ing.."));
	}
	else
	{
		//���ݽ���
		int32 RandAttack = FMath::RandRange(1, 2);
		if (RandAttack==1)
		{
			currentTime = 0;
			bRushdirCheck = false;
			mState = EEnemyState::RushAttack;
			
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Normal Attack"));
			mState = EEnemyState::Idle;
		}

		//��������or ����
	}
}

void UPKM_OLDDSFSM::DamageState()
{

}

void UPKM_OLDDSFSM::DieState()
{

}

void UPKM_OLDDSFSM::Moving(float speed,FVector dir)
{
	dir.Normalize();
	FVector P0 = Me->GetActorLocation();
	if (MovingSpeed>speed)//����
	{
		if (MovingSpeed - 5<speed)
		{
			MovingSpeed = speed;
		}
		else {
			MovingSpeed = MovingSpeed - 5;
		}
	}
	else if (MovingSpeed < speed)//�ָ��ϳ�
	{
		if (MovingSpeed + 5>speed)
		{
			MovingSpeed = speed;
		}
		else
		{
			MovingSpeed = MovingSpeed + 5;
		}
	}
	FVector vt = dir * MovingSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	FVector Forward = Me->GetActorForwardVector();
	Forward = FMath::Lerp<FVector, float>(Forward, dir, 5 * GetWorld()->DeltaTimeSeconds);
	GetOwner()->SetActorRotation(Forward.Rotation());
	GetOwner()->SetActorLocation(P);
}

void UPKM_OLDDSFSM::BackStepState()
{
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	float FastTime=0.1;
	float SlowTime=0.8;
	float EndTime=1;
	FVector P0 = Me->GetActorLocation();
	direction = Target->GetActorLocation() - P0;
	//FVector Forward = Me->GetActorForwardVector();
	//Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
	//GetOwner()->SetActorRotation(Forward.Rotation());
	direction.Normalize();
	if (currentTime<=FastTime)// 0~0.1
	{
		bBackStepAnimCheck = true;
		BackStepSpeed = 1000 * sqrt(currentTime/FastTime);
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * -1 *BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime < SlowTime)// 0.1~0.8
	{
		BackStepSpeed = 1000;
		FVector vt = direction * -1 * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime<EndTime)// 0.8<= CT<1.0
	{	//				1000 * (1-x2)
		BackStepSpeed = 1000 * (1 - ((currentTime - SlowTime) / (EndTime - SlowTime)) * ((currentTime - SlowTime) / (EndTime - SlowTime)));
		//UE_LOG(LogTemp, Log, TEXT("TIme=%f BSS2 %f ,((currentTime-1)/2)=%f, (1-((currentTime-1)/2)*((currentTime - 1)/ 2)) %f"), currentTime, BackStepSpeed, (1 - ((currentTime - 2.0f) / 1.0f) * ((currentTime - 2.0f) / 1.0f)));
		FVector vt = direction * -1 * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("BackSEnd"));
		bBackStepAnimCheck = false;
		mState = EEnemyState::Idle;
	}
}
void UPKM_OLDDSFSM::RushAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	float FastTime = 0.1;
	float SlowTime = 0.8;
	float EndTime = 1;
	FVector P0 = Me->GetActorLocation();
	if(!bRushdirCheck){
		direction = Target->GetActorLocation() - P0;
		direction.Normalize();
		bRushdirCheck = true;
	}
	//FVector Forward = Me->GetActorForwardVector();
	//Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
	//GetOwner()->SetActorRotation(Forward.Rotation());
	if (currentTime <= FastTime)// 0~0.1
	{
		bRushAnimCheck = true;
		BackStepSpeed = 1000 * sqrt(currentTime / FastTime);
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction  * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime < SlowTime)// 0.1~0.8
	{
		BackStepSpeed = 1000;
		FVector vt = direction  * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime < EndTime)// 0.8<= CT<1.0
	{	//				1000 * (1-x2)
		BackStepSpeed = 1000 * (1 - ((currentTime - SlowTime) / (EndTime - SlowTime)) * ((currentTime - SlowTime) / (EndTime - SlowTime)));
		//UE_LOG(LogTemp, Log, TEXT("TIme=%f BSS2 %f ,((currentTime-1)/2)=%f, (1-((currentTime-1)/2)*((currentTime - 1)/ 2)) %f"), currentTime, BackStepSpeed, (1 - ((currentTime - 2.0f) / 1.0f) * ((currentTime - 2.0f) / 1.0f)));
		FVector vt = direction  * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else
	{
		bRushAnimCheck = false;
		UE_LOG(LogTemp, Log, TEXT("RushEnd"));
		mState = EEnemyState::Idle;
	}
}

void UPKM_OLDDSFSM::ReciveDamage(float value)
{
	if (MyAttacktype == 1) //����Ÿ��1�� ���۾Ƹ���
	{
		if (HP-value>0)
		{
			HP -= value;
		}
		else
		{
			HP = 0;
			mState = EEnemyState::Die;
		}
	}
}

float UPKM_OLDDSFSM::GiveDamage()
{
	switch (MyAttacktype)
	{
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;
	default:
		return -1;
		break;
	}
}

