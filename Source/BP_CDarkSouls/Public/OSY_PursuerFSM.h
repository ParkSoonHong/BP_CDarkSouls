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
	Attack1_1 UMETA(DisplayName = "Attack1_1 State"),
	Attack1_2 UMETA(DisplayName = "Attack1_2 State"),
	Attack1_3 UMETA(DisplayName = "Attack1_3 State"),
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
	void Attack1_1State();
	void Attack1_2State();
	void Attack1_3State();
	void Attack2State();
	void DamageState();
	void DieState();

public: //Idle 속성
	// 필요속성 : 플레이어와의 거리, 대시거리, 무브거리,어택거리, 백스텝거리
	float RushDistance = 5000;
	float AttackStartDistance = 2000;
	float BackstepStartDistance = 200;
	float RushStartDistance = 600;
	float RushAttackDistance = 500;


	float rushstartDistance= 1000;
	float attackstartDistance = 300;
	float backstepstartDiatacne = 100;

	float MoveRange =1000;
	float AttackRange = 250;
	float BackstepRange = 150;


public: // Attack 속성

	bool bAttackSelect = false;
	float pattern;
	bool bAttackdirOk = false;
	FVector	Attackdir;
public: //피격 속성
	UFUNCTION()
	void ReciveDamage(float value);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 PURSUERHP=10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 PURSUERMAXHP=10;

public:	//애니관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UOSY_AnimInstance*anim;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystem *DieFactory;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystemComponent *DieEffect;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystem *DieEndStartFactory;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystemComponent *DieEndStartEffect;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystem *DieEndFactory;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystemComponent *DieEndEffect;

	UPROPERTY(EditDefaultsOnly,Category="Sounds")
	class USoundBase *RushSound;
	bool bRushSound=false;

	UPROPERTY(EditDefaultsOnly,Category="Sounds")
	class USoundBase *RushAttackSound;
	bool bRushAttackSound=false;
	

	
};
