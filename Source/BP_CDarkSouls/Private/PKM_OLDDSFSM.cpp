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
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
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
	if (distance<MoveRange)
	{
		UE_LOG(LogTemp, Log, TEXT("switchMove"));
		mState = EEnemyState::Move;
	}
}

void UPKM_OLDDSFSM::MoveState()
{
	FVector Forward = Me->GetActorForwardVector();
	//direction = FVector(0, 0, 0);
	if (Target != nullptr)
	{
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		distance=direction.Size();
	}
	if (distance < attackRange)
	{
		currentTime = 0;
		UE_LOG(LogTemp, Log, TEXT("switchAttack"));
		mState = EEnemyState::Attack;
	}
	else {
		direction.Normalize();
		FVector P0 = Me->GetActorLocation();
		FVector vt = direction * 200 * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
		GetOwner()->SetActorRotation(Forward.Rotation());
		GetOwner()->SetActorLocation(P);
	}
	
}

void UPKM_OLDDSFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime>idleDelayTime)
	{
		UE_LOG(LogTemp, Log, TEXT("attack"));
		mState = EEnemyState::Idle;
	}
}

void UPKM_OLDDSFSM::DamageState()
{

}

void UPKM_OLDDSFSM::DieState()
{

}
