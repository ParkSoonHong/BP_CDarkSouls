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

	// �ʿ�Ӽ� : ���ð�,����ð�
	UPROPERTY(EditAnywhere, Category ="FSM")
	float idleDelayTime = 2;
	float currentTIme = 0;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float delayTime;
	float DelayTime= 2;
	float RushDelayTime= 1;
	float stopTime=2;
	

	// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�, ����
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Backspeed;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Walkspeed = 500;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float Rushspeed;
	// ���� �����ϰ� �ִ� ����
	UPROPERTY()
	class AOSY_Pursuer *me;
	// Ÿ�� ����
	UPROPERTY(EditAnywhere)
	class APSH_CPlayer *Target;

public:	 // ���� �Լ�
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

public: //Idle �Ӽ�
	// �ʿ�Ӽ� : �÷��̾���� �Ÿ�, ��ðŸ�, ����Ÿ�,���ðŸ�, �齺�ܰŸ�
	float RushDistance = 5000;
	float AttackStartDistance = 2000;
	float BackstepStartDistance = 200;
	float RushStartDistance = 1000;
	float RushAttackDistance = 500;

public: // Attack �Ӽ�

	bool bAttackSelect = false;
	float pattern;
	bool bAttackdirOk = false;
	FVector	Attackdir;
public: //�ǰ� �Ӽ�
	UFUNCTION()
	void ReciveDamage(float value);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 HP=10;

public:	//�ִϰ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UOSY_AnimInstance*anim;
	

	
};
