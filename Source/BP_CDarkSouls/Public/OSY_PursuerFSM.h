// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OSY_PursuerFSM.generated.h"

UENUM(BlueprintType)
enum class EEnmeyState :uint8
{
	Idle UMETA(DisplayName = "Idle State"),
	Backstep UMETA(DisplayName = "Backstep State"),
	Walk UMETA(DisplayName = "Walk State"),
	Rush UMETA(DisplayName = "Rush State"),
	Attack UMETA(DisplayName = "Attack State"),
	Damage UMETA(DisplayName = "Damage State"),
	Die UMETA(DisplayName = "Die State")
};

UENUM(BlueprintType)
enum class EEnmeyAttackState :uint8
{
	AttackBasic UMETA(DisplayName="Attack Basic State"),
	Attack1 UMETA(DisplayName = "Attack1 State"),
	Attack2 UMETA(DisplayName = "Attack2 State"),
	Attack3 UMETA(DisplayName = "Attack3 State")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BP_CDARKSOULS_API UOSY_PursuerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOSY_PursuerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="FSM")
	EEnmeyState mState = EEnmeyState::Idle;

	// 필요속성 : 대기시간,경과시간
	UPROPERTY(EditAnywhere, Category ="FSM")
	float idleDelayTime = 2;
	float currentTIme = 0;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float delyTime =0;
	// 필요속성 : 타겟, 이동속도, 방향
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Backspeed;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Walkspeed = 3000;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Rushspeed;
	// 나를 소유하고 있는 액터
	UPROPERTY()
	class AOSY_Pursuer *me;
	// 타겟 선언
	UPROPERTY(EditAnywhere)
	class AUPlayer *Target;

public:	 // 상태 함수
	void IdleState();
	void BackstepState();
	void WalkState();
	void RushState();
	void AttackState();
	void DamageState();
	void DieState();

public: //Idle 속성
	// 필요속성 : 플레이어와의 거리, 대시거리, 무브거리,어택거리, 백스텝거리
	float RushDistance = 5000;
	float RushStartDistance = 1000;
	float AttackStartDistance = 150;
	float BackstepStartDistance = 70;


public: // Attack 상태함수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="FSM")
	EEnmeyAttackState mAttackState = EEnmeyAttackState::AttackBasic;

	void AttackBasic();
	void Attack1();
	void Attack2();
	void Attack3();

public: // Attack 속성
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void LowerAndRaiseWeapon();
		
};
