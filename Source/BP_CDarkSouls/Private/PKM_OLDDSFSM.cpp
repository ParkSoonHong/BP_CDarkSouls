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
	if (bDebugRange)
	{
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), attackRange, 100, FColor::Red, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), MoveRange, 100, FColor::Yellow, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), RunRange, 100, FColor::Blue, false, -1, 0, 2);
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
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
	int checkdis = distance;
	if (distance<attackRange)
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
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
	}
	if (distance < attackRange)
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
		Moving(500, direction);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("switchIdle"));
		mState = EEnemyState::Idle;
	}

}

void UPKM_OLDDSFSM::MoveState()
{
	//direction = FVector(0, 0, 0);
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
	}
	if (distance < attackRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchAttack"));
		currentTime = 0;
		mState = EEnemyState::Attack;
	}
	else if (distance < MoveRange)
	{
		Moving(200, direction);
	}
	else if (distance < RunRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchRun"));
		mState = EEnemyState::Run;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("switchIdle"));
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
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance = direction.Size();
		FVector Forward = Me->GetActorForwardVector();
		Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
		GetOwner()->SetActorRotation(Forward.Rotation());
	}
	if (currentTime>idleDelayTime)
	{
		if (distance < attackRange)
		{
			currentTime = 0;
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
		else {
			UE_LOG(LogTemp, Log, TEXT("switchIdle"));
			mState = EEnemyState::Idle;
		}
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
	
	direction.Normalize();
	FVector P0 = Me->GetActorLocation();
	FVector vt = direction * speed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	FVector Forward = Me->GetActorForwardVector();
	Forward = FMath::Lerp<FVector, float>(Forward, dir, 5 * GetWorld()->DeltaTimeSeconds);
	GetOwner()->SetActorRotation(Forward.Rotation());
	GetOwner()->SetActorLocation(P);
}
