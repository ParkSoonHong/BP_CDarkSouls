// Fill out your copyright notice in the Description page of Project Settings.


#include "PKM_OLDDSFSM.h"
#include "PSH_CPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "PKM_OLDDS.h"
#include "Components/CapsuleComponent.h"
#include <Particles/ParticleSystemComponent.h>
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
	Target = Cast<APSH_CPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), APSH_CPlayer::StaticClass()));
	Me = Cast<APKM_OLDDS>(GetOwner());
	Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Called every frame
void UPKM_OLDDSFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	/*if (bDebugRange)
	{

	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), BackRange, 100, FColor::Black, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), attackRange, 100, FColor::Red, false, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Me->GetActorLocation(), Range, 100, FColor::Yellow, false, -1, 0, 2);
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
	case  EEnemyState::StingAttack:
		StingAttackState();
		break;
	case  EEnemyState::SwingAttack:
		SwingAttackState();
		break;
	case  EEnemyState::RangeAttack:
		RangeAttackState();
		break;
	case  EEnemyState::StingTwoAttack:
		StingTwoAttackState();
		break;
	case  EEnemyState::RaiseAttack:
		RaiseAttackState();
		break;
	case  EEnemyState::TakeDownAttack:
		TakeDownAttackState();
		break;
	case  EEnemyState::TestAttack:
		TestAttackState();
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

	if (distance < BackRange) {
		if (FVector::DotProduct(direction, Me->GetActorForwardVector()) >= 0) {
			UE_LOG(LogTemp, Log, TEXT("Go BackStep"));
			currentTime = 0;
			BackStepSpeed = 0;
			//방향 완전히 돌려라
			GetOwner()->SetActorRotation(direction.Rotation());
			//--------------------------------------------------------------------------------------------
			mState = EEnemyState::BackStep;

			UE_LOG(LogTemp, Log, TEXT("memset Time=%f"), currentTime);
		}
		else 
		{	//방향돌리기
				FVector Forward = Me->GetActorForwardVector();
				Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
				GetOwner()->SetActorRotation(Forward.Rotation());
				UE_LOG(LogTemp, Log, TEXT("Turn.."));
		}
	}
	else if (distance < attackRange)
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
			if (FVector::DotProduct(direction, Me->GetActorForwardVector()) >= 0) {
				UE_LOG(LogTemp, Log, TEXT("Go BackStep"));
				currentTime = 0;
				BackStepSpeed = 0;
				//방향 완전히 돌려라
				GetOwner()->SetActorRotation(direction.Rotation());
				//--------------------------------------------------------------------------------------------
				mState = EEnemyState::BackStep;

				UE_LOG(LogTemp, Log, TEXT("memset Time=%f"), currentTime);
			}
			else {
				FVector Forward = Me->GetActorForwardVector();
				Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
				GetOwner()->SetActorRotation(Forward.Rotation());
				UE_LOG(LogTemp, Log, TEXT("Turn.."));
			}
		}
		else if (distance < attackRange)
		{
			UE_LOG(LogTemp, Log, TEXT("switchAttack"));
			currentTime = 0;
			bRunAnimCheck = false;
			mState = EEnemyState::Attack;
		}
		else if (distance < MoveRange)
		{
			int32 RandRushRun = FMath::RandRange(1, 2);
			if (RandRushRun == 1)
			{
				bRunAnimCheck = false;
				currentTime = 0;
				bRushdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::RushAttack;
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("switchMove"));
				bRunAnimCheck = false;
				mState = EEnemyState::Move;
			}
		}
		else if (distance < RunRange)
		{
			Moving(200, direction);
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
			UE_LOG(LogTemp, Log, TEXT("Go BackStep"));
			currentTime = 0;
			BackStepSpeed = 0;
			//방향 완전히 돌려라
			GetOwner()->SetActorRotation(direction.Rotation());
			//--------------------------------------------------------------------------------------------
			bWalkAnimCheck = false;
			mState = EEnemyState::BackStep;

			UE_LOG(LogTemp, Log, TEXT("memset Time=%f"), currentTime);
		}
		else {
				FVector Forward = Me->GetActorForwardVector();
				Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
				GetOwner()->SetActorRotation(Forward.Rotation());
				UE_LOG(LogTemp, Log, TEXT("Turn.."));
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
		if (MovingSpeed > 200)
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
	if (currentTime < FixRotTime)
	{
		FVector Forward = Me->GetActorForwardVector();
		Forward = FMath::Lerp<FVector, float>(Forward, direction, 0.1 * GetWorld()->DeltaTimeSeconds);
		GetOwner()->SetActorRotation(Forward.Rotation());
		//UE_LOG(LogTemp, Log, TEXT("fixRot ing.."));
	}
	else
	{
		//공격시작
		/*
		1 스윙
		2 스팅
		3 투핸드스팅
		4 레이즈업
		5 테이크다운
		6 러쉬
		7 래인지어택
		*/
		int32 RandAttack = FMath::RandRange(1, 7);
		if (RandAttack == 1)
		{
			currentTime = 0;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::SwingAttack;
		}
		else if (RandAttack==2)
		{
			currentTime = 0;
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::StingAttack;
		}
		else if (RandAttack == 3)
		{
			currentTime = 0;
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::StingTwoAttack;
		}
		else if (RandAttack == 4)
		{
			currentTime = 0;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::RaiseAttack;
		}
		else if (RandAttack == 5)
		{
			currentTime = 0;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::TakeDownAttack;
		}
		else if (RandAttack == 6)
		{
			currentTime = 0;
			bRushdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::RushAttack;

		}
		else if (RandAttack == 7)
		{
			currentTime = 0;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			bRangeAttackHit = false;
			mState = EEnemyState::RangeAttack;
		}
		else {
			currentTime = 0;
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mState = EEnemyState::TestAttack;
		}
		//공격종료or 피함
	}
}

void UPKM_OLDDSFSM::DamageState()
{

}

void UPKM_OLDDSFSM::DieState()
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	if (currentTime>1)
	{
		if (DieEndEffect==nullptr)
		{
			FVector DEELoc = Me->GetActorLocation();
			DEELoc.Z -= 200;
			DieEndEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieEndFactory,DEELoc);
			DieEndEffect->SetWorldScale3D(FVector(2,2,2));
		}
	}
	if (currentTime > 6)
	{
		DieEndStartEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieEndStartFactory, Me->GetActorLocation());
		DieEndStartEffect->SetWorldScale3D(FVector(10,10,0.1));
		//DieEndEffect->DestroyComponent();
		//DieEndEffect->EndPlay(EEndPlayReason::Destroyed);
		Me->Destroy();
	}
}

void UPKM_OLDDSFSM::Moving(float speed, FVector dir)
{
	dir.Normalize();
	FVector P0 = Me->GetActorLocation();
	if (MovingSpeed > speed)//감속
	{
		if (MovingSpeed - 5 < speed)
		{
			MovingSpeed = speed;
		}
		else {
			MovingSpeed = MovingSpeed - 5;
		}
	}
	else if (MovingSpeed < speed)//애매하네
	{
		if (MovingSpeed + 5 > speed)
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
	//GetOwner()->SetActorRotation(Forward.Rotation());
	GetOwner()->SetActorRotation(dir.Rotation());
	GetOwner()->SetActorLocation(P);
}

void UPKM_OLDDSFSM::BackStepState()
{

	currentTime += GetWorld()->DeltaTimeSeconds;
	float FastTime = 0.1;
	float SlowTime = 0.8;
	float EndTime = 1;
	FVector P0 = Me->GetActorLocation();
	direction = Target->GetActorLocation() - P0;
	direction.Z = 0;
	//FVector Forward = Me->GetActorForwardVector();
	//Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
	//GetOwner()->SetActorRotation(Forward.Rotation());
	direction.Normalize();
	if (currentTime <= FastTime)// 0~0.1
	{
		bBackStepAnimCheck = true;
		BackStepSpeed = 1000 * sqrt(currentTime / FastTime);
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * -1 * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
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
	else if (currentTime < EndTime)// 0.8<= CT<1.0
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
	float RushDelay = 0.3;
	float FastTime = 0.5;
	float SlowTime = 0.7;
	float EndTime = 0.8;
	FVector P0 = Me->GetActorLocation();
	if (!bRushdirCheck) {
		direction = Target->GetActorLocation() - P0;
		direction.Z = 0;
		direction.Normalize();
		bRushdirCheck = true;
		Me->HitComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FVector MyLoc = Me->GetActorLocation();
		FVector RushELoc = MyLoc;
		RushELoc.Z = RushELoc.Z + 60;
		RushAttackEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RushAttackFactory, RushELoc);
		RushAttackEffect->SetWorldScale3D(FVector(0,0,0));
	}
	//FVector Forward = Me->GetActorForwardVector();
	//Forward = FMath::Lerp<FVector, float>(Forward, direction, 5 * GetWorld()->DeltaTimeSeconds);
	//GetOwner()->SetActorRotation(Forward.Rotation());
	if (currentTime<=RushDelay)
	{
		bRushAnimCheck = true;
		RushAttackEffect->SetWorldScale3D(FVector(3*(currentTime/0.3),3* (currentTime / 0.3),3* (currentTime / 0.3)));
	}
	else if (currentTime <= FastTime)// 0~0.1
	{
		BackStepSpeed = 2000 * sqrt(currentTime / FastTime);
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
		RushAttackEffect->SetWorldLocation(FVector(P.X,P.Y,P.Z+60));
	}
	else if (currentTime < SlowTime)// 0.1~0.8
	{
		BackStepSpeed = 2000;
		FVector vt = direction * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
		RushAttackEffect->SetWorldLocation(FVector(P.X, P.Y, P.Z + 60));
	}
	else if (currentTime < EndTime)// 0.8<= CT<1.0
	{	//				1000 * (1-x2)
		BackStepSpeed = 2000 * (1 - ((currentTime - SlowTime) / (EndTime - SlowTime)) * ((currentTime - SlowTime) / (EndTime - SlowTime)));
		//UE_LOG(LogTemp, Log, TEXT("TIme=%f BSS2 %f ,((currentTime-1)/2)=%f, (1-((currentTime-1)/2)*((currentTime - 1)/ 2)) %f"), currentTime, BackStepSpeed, (1 - ((currentTime - 2.0f) / 1.0f) * ((currentTime - 2.0f) / 1.0f)));
		FVector vt = direction * BackStepSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
		RushAttackEffect->SetWorldLocation(FVector(P.X, P.Y, P.Z + 60));
	}
	else if (currentTime < 3) {
		bRushAnimCheck = false;
		Me->HitComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RushAttackEffect->SetWorldScale3D(FVector(3-3*(currentTime-0.8)/2.2,3-3*(currentTime - 0.8) / 2.2,3-3* (currentTime - 0.8) / 2.2));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("RushEnd"));
		Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EEnemyState::Idle;
	}
}

void UPKM_OLDDSFSM::StingAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	FVector P0 = Me->GetActorLocation();
	if (!bStingdirCheck) {
		bStingAttackAnimCheck = true;
		UE_LOG(LogTemp, Log, TEXT("StingStart"));
		direction = Target->GetActorLocation() - P0;
		direction.Z = 0;
		StingSpeed = 500;
		direction.Normalize();
		bStingdirCheck = true;
		Me->SetActorRotation(direction.Rotation()*(currentTime/0.1));
	}
	else if (currentTime < 0.1)
	{
		direction = Target->GetActorLocation() - P0;
		direction.Normalize();
		Me->SetActorRotation(direction.Rotation());
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * (StingSpeed*currentTime/0.1) * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime < 0.5)
	{
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * StingSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime < 1.5)//후딜레이 0.5초
	{

	}
	else
	{	
		if (ComboCount < 1)
		{
			int32 RandCombo = FMath::RandRange(1, 7);
	
			if (RandCombo == 1)
			{
				ComboCount++;
				bStingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스윙으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::SwingAttack;
			}
			else if (RandCombo == 2)
			{
				ComboCount = 0;
				bStingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
			else if (RandCombo == 3)
			{
				ComboCount++;
				bStingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//투핸드가
				currentTime = 0;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::StingTwoAttack;
			}
			else if (RandCombo == 4)
			{
				ComboCount++;
				bStingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//raiseup으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::RaiseAttack;
			}
			else if (RandCombo == 5)
			{
				ComboCount++;
				bStingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//테이크다운으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::TakeDownAttack;
			}
			else
			{
				ComboCount = 0;
				bStingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
		}
		else
		{
			ComboCount = 0;
			bStingAttackAnimCheck = false;
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mState = EEnemyState::Idle;
		}
	}
}

void UPKM_OLDDSFSM::StingTwoAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	FVector P0 = Me->GetActorLocation();
	if (!bStingdirCheck) {
		UE_LOG(LogTemp, Log, TEXT("TEST TWOHAND"));
		bStingTwoAttackAnimCheck = true;
		UE_LOG(LogTemp, Log, TEXT("StingStart"));
		direction = Target->GetActorLocation() - P0;
		direction.Z = 0;
		Me->SetActorRotation(direction.Rotation());
		StingSpeed = 1000;
		direction.Normalize();
		bStingdirCheck = true;
	}
	else if (currentTime < 0.1)
	{
		direction = Target->GetActorLocation() - P0;
		direction.Normalize();
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * (StingSpeed * currentTime / 0.1) * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);

		Me->SetActorRotation(direction.Rotation()*(currentTime/0.1));
	}
	else if (currentTime < 0.5)
	{
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		FVector vt = direction * StingSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);
	}
	else if (currentTime < 1.4)//후딜레이 0.5초
	{

	}
	else
	{
		if (ComboCount<1)
		{
			int32 RandCombo = FMath::RandRange(1,7);
			if (RandCombo==1)
			{
				ComboCount++;
				bStingTwoAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스윙으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::SwingAttack;
			}
			else if (RandCombo==2)
			{
				ComboCount++;
				bStingTwoAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스팅1으로가
				currentTime = 0;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::StingAttack;
			}
			else if (RandCombo == 3)
			{
				ComboCount = 0;
				bStingTwoAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
			else if (RandCombo==4)
			{
				ComboCount++;
				bStingTwoAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//raiseup으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::RaiseAttack;
			}
			else if (RandCombo == 5)
			{
				ComboCount++;
				bStingTwoAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//테이크다운으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::TakeDownAttack;
			}
			else
			{
				ComboCount = 0;
				bStingTwoAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
		}
		else
		{
			ComboCount = 0;
			bStingTwoAttackAnimCheck = false;
			UE_LOG(LogTemp, Log, TEXT("StingEnd"));
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mState = EEnemyState::Idle;
		}
	}
}

void UPKM_OLDDSFSM::SwingAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime < 0.4)//들어올리기
	{	
		bSwingAttackAnimCheck = true;
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		direction.Z = 0;
		Me->SetActorRotation(direction.Rotation() * (currentTime / 0.4));
		//Me->spearComp->SetRelativeRotation(FRotator(0, 150 * currentTime / 0.4f, -20 * currentTime / 0.4f));
	}
	else if (currentTime < 0.5)//휘두르기
	{
		//Me->spearComp->SetRelativeRotation(FRotator(0, 150 -(300* (currentTime-0.4) / 0.1f), -20 +(50* (currentTime - 0.4) / 0.1f)));
	}
	else if (currentTime < 0.7)//후딜레이 0.2초
	{

	}
	else if (currentTime < 1.1)
	{
		//Me->spearComp->SetRelativeRotation(FRotator(0, -150, 30-(50*(currentTime-0.7)/0.2f)));
	}
	else if (currentTime < 1.2)
	{
		//Me->spearComp->SetRelativeRotation(FRotator(0, -150 + (300 * (currentTime - 1.1) / 0.1f), -20 + (50 * (currentTime - 1.1) / 0.1f)));
	}
	else if (currentTime < 1.06) {

	}
	else
	{

		if (ComboCount < 1)
		{
			int32 RandCombo = FMath::RandRange(1, 7);
			if (RandCombo == 1)
			{
				ComboCount = 0;
				bSwingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
				//스윙으로가
			}
			else if (RandCombo == 2)
			{
				ComboCount++;
				bSwingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스팅1으로가
				currentTime = 0;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::StingAttack;
			}
			else if (RandCombo == 3)
			{
				ComboCount++;
				bSwingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//투핸드어택가
				currentTime = 0;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::StingTwoAttack;
			}
			else if (RandCombo == 4)
			{
				ComboCount++;
				bSwingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//raiseup으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::RaiseAttack;
			}
			else if (RandCombo == 5)
			{
				ComboCount++;
				bSwingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//테이크다운으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::TakeDownAttack;
			}
			else
			{
				ComboCount = 0;
				bSwingAttackAnimCheck = false;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
		}
		else
		{
			ComboCount = 0;
			bSwingAttackAnimCheck = false;
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mState = EEnemyState::Idle;
		}
	}
}

void UPKM_OLDDSFSM::RangeAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (Target!=nullptr)
	{
		FVector RangeDistance = Me->GetActorLocation() - Target->GetActorLocation();
		if (currentTime<1)
		{
			//이펙트 생성 
			if (!bRangeAttackEffect)
			{
				FVector MyLoc = Me->GetActorLocation();
				FVector StormLoc = MyLoc;
				StormLoc.Z = StormLoc.Z - 90;
				FVector BallLoc = MyLoc;
				BallLoc.Z = BallLoc.Z + 50;
				RangeAttackEffect=UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RangeAttackFactory,StormLoc);
				RangeBallEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RangeAttackBallFactory,BallLoc);
				bRangeAttackEffect = true;
			}
			RangeAttackEffect->SetWorldScale3D(FVector(0.3*currentTime, 0.3*currentTime, 1));
			RangeBallEffect->SetWorldScale3D(FVector(2*currentTime, 2*currentTime, 1));
		}
		else if (currentTime < 2)//벌리기
		{
			bRangeAttackAnimCheck = true;
			RangeAttackEffect->SetWorldScale3D(FVector(4* ((currentTime - 1) / 1), 4* ((currentTime - 1) / 1), 1));
			FVector BallP0 = RangeBallEffect->GetComponentLocation();
			FVector Balldis = Me->GetActorForwardVector();
			FVector Ballvt=180*Balldis*GetWorld()->DeltaRealTimeSeconds;
			FVector BallP=BallP0+Ballvt;
			RangeBallEffect->SetWorldLocation(BallP);
			if (!bRangeAttackHit)
			{
				if (RangeDistance.Size() < 800 * ((currentTime - 1) / 1))
				{
					Target->Damaged(1);
					bRangeAttackHit = true;
				}
			}
		}
		else if (currentTime < 3)//좁히기
		{
			RangeAttackEffect->SetWorldScale3D(FVector(4-4 * ((currentTime - 2) / 1), 4-4 * ((currentTime - 2) / 1), 1));
			if (!bRangeAttackHit)
			{
				if (RangeDistance.Size() < 800 - 800 * ((currentTime - 2) / 1))
				{
					Target->Damaged(1);
					bRangeAttackHit = true;
				}
			}
		}
		else if (currentTime < 4) {
			RangeBallEffect->SetWorldScale3D(FVector(2-2 * ((currentTime-3)/1), 2-2 * ((currentTime - 3) / 1), 1-1*((currentTime - 3) / 1)));
			//후딜1초
			RangeAttackEffect->DestroyComponent();

		}
		else
		{
			RangeBallEffect->DestroyComponent();
			bRangeAttackEffect = false;
			//RangeAttackEffect->FinishDestroy();
			bRangeAttackAnimCheck = false;
			UE_LOG(LogTemp, Log, TEXT("RangeEnd"));
			mState = EEnemyState::Idle;
		}
	}
}

void UPKM_OLDDSFSM::RaiseAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime < 0.4)//들어올리기
	{
		bRaiseAttackAnimCheck = true;
		direction = Target->GetActorLocation() - Me->GetActorLocation();
		Me->SetActorRotation(direction.Rotation()*(currentTime/0.4));
		//Me->spearComp->SetRelativeRotation(FRotator(0, 150 * currentTime / 0.4f, -20 * currentTime / 0.4f));
	}
	else if (currentTime < 0.5)//휘두르기
	{
		//Me->spearComp->SetRelativeRotation(FRotator(0, 150 -(300* (currentTime-0.4) / 0.1f), -20 +(50* (currentTime - 0.4) / 0.1f)));
	}
	else if (currentTime < 0.7)//후딜레이 0.2초
	{

	}
	else if (currentTime < 1)
	{
		//Me->spearComp->SetRelativeRotation(FRotator(0, -150, 30-(50*(currentTime-0.7)/0.2f)));
	}
	else
	{
		if (ComboCount < 1)
		{
			int32 RandCombo = FMath::RandRange(1, 7);
			if (RandCombo == 1)
			{
				ComboCount++;
				bRaiseAttackAnimCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스윙으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::SwingAttack;
			}
			else if (RandCombo == 2)
			{
				ComboCount++;
				bRaiseAttackAnimCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스팅1으로가
				currentTime = 0;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::StingAttack;
			}
			else if (RandCombo == 3)
			{
				ComboCount++;
				bRaiseAttackAnimCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//스팅2으로가
				currentTime = 0;
				bStingdirCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::StingTwoAttack;
			}
			else if (RandCombo == 4)
			{
				ComboCount = 0;
				bRaiseAttackAnimCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
			else if (RandCombo == 5)
			{
				ComboCount++;
				bRaiseAttackAnimCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//테이크다운으로가
				currentTime = 0;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				mState = EEnemyState::TakeDownAttack;
			}
			else
			{
				ComboCount = 0;
				bRaiseAttackAnimCheck = false;
				Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				mState = EEnemyState::Idle;
			}
		}
		else
		{
			ComboCount = 0;
			bRaiseAttackAnimCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mState = EEnemyState::Idle;
		}
	}
}

void UPKM_OLDDSFSM::TakeDownAttackState()
{
	ComboCount = 0;
	bTakeDownAttackAnimCheck = false;
	Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mState = EEnemyState::Idle;
}

void UPKM_OLDDSFSM::TestAttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	FVector P0 = Me->GetActorLocation();
	if (!bStingdirCheck) {
		bTestAttackAnimCheck = true;
		UE_LOG(LogTemp, Log, TEXT("TEstStingStart"));
		direction = Target->GetActorLocation() - P0;
		StingSpeed = 3000;
		direction.Normalize();
		bStingdirCheck = true;
		bTestAttackAnimPlayingEnd = false;
	}
	else if (currentTime < 0.1)
	{
		//UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
	/*	FVector vt = direction * (StingSpeed * currentTime / 0.1) * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		Me->SetActorLocation(P);*/
	}
	else if (currentTime < 4.23)
	{
		////UE_LOG(LogTemp, Log, TEXT("Time=%f BSS1%f"),currentTime,BackStepSpeed);
		//FVector vt = direction * StingSpeed * GetWorld()->DeltaTimeSeconds;
		//FVector P = P0 + vt;
		//Me->SetActorLocation(P);
	}
	else
	{
		if (bTestAttackAnimPlayingEnd)
		{
			bTestAttackAnimCheck = false;
			UE_LOG(LogTemp, Log, TEXT("StingEnd"));
			bStingdirCheck = false;
			Me->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FVector vt = direction * 1398.4; // 1300 임의값
			FVector P = P0 + vt;
				//UE_LOG(LogTemp, Log, TEXT("dis%f"),) ;
			Me->SetActorLocation(P);
			mState = EEnemyState::Idle;
		}
	}
}

//상태끝
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UPKM_OLDDSFSM::ReciveDamage(float value)
{
		if (HP - value > 0)
		{
			HP -= value;
			UE_LOG(LogTemp, Log, TEXT("OLDDS HP=%d"),HP);
		}
		else
		{
			HP = 0;
			bBackStepAnimCheck = false;
			bRushAnimCheck = false;
			bWalkAnimCheck = false;
			bRunAnimCheck = false;
			bRangeAttackAnimCheck = false;
			bStingAttackAnimCheck = false;
			bSwingAttackAnimCheck = false;
			bStingTwoAttackAnimCheck = false;
			bRaiseAttackAnimCheck = false;
			bTakeDownAttackAnimCheck = false;
			bTestAttackAnimCheck = false;
			bDieAnimCheck = true;
			DieEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieFactory, Me->GetActorLocation());
			Me->HitComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (RangeAttackEffect!=nullptr)
			{
				RangeAttackEffect->DestroyComponent();
			}
			if (RangeBallEffect != nullptr)
			{
				RangeBallEffect->DestroyComponent();
			}
			if (RushAttackEffect != nullptr)
			{
				RushAttackEffect->DestroyComponent();
			}
			currentTime = 0;
			UE_LOG(LogTemp, Log, TEXT("go die"));
			mState = EEnemyState::Die;
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