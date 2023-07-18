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
	Attack UMETA(DisplayName = "Attack State"),
	Rush UMETA(DisplayName = "Rush State"),
	RushAttack UMETA(DisplayName = "RushAttack State"),
	Attack1 UMETA(DisplayName = "Attack1 State"),
	Attack2 UMETA(DisplayName = "Attack2 State"),
	Damage UMETA(DisplayName = "Damage State"),
	Die UMETA(DisplayName = "Die State")
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
	float delayTime;
	float DelayTime= 2;
	float RushDelayTime= 1;
	float stopTime=2;
	

	// 필요속성 : 타겟, 이동속도, 방향
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Backspeed;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Walkspeed = 500;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Rushspeed;
	// 나를 소유하고 있는 액터
	UPROPERTY()
	class AOSY_Pursuer *me;
	// 타겟 선언
	UPROPERTY(EditAnywhere)
	class APSH_CPlayer *Target;

public:	 // 상태 함수
	void IdleState();
	void BackstepState();
	void WalkState();
	void AttackState();
	void RushState();
	void RushAttackState();
	void Attack1State();
	void Attack2State();
	void DamageState();
	void DieState();

public: //Idle 속성
	// 필요속성 : 플레이어와의 거리, 대시거리, 무브거리,어택거리, 백스텝거리
	float RushDistance = 5000;
	float AttackStartDistance = 2000;
	float BackstepStartDistance = 200;
	float RushStartDistance = 1000;
	float RushAttackDistance = 500;

public: // Attack 속성

	bool bAttackSelect = false;
	float pattern;
	bool bAttackdirOk = false;
	FVector	Attackdir;
public: //피격 속성
	UFUNCTION()
	void ReciveDamage(float value);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 HP=10;

public:	//애니관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UOSY_AnimInstance*anim;
	

	
};
