// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PKM_OLDDSFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle State"),
	Move,
	Run,
	Attack,
	Damage,
	Die,
	BackStep,
	RushAttack,
	StingAttack,
	StingTwoAttack,
	SwingAttack,
	RangeAttack,
	RaiseAttack,
	TakeDownAttack,
	TestAttack,

};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BP_CDARKSOULS_API UPKM_OLDDSFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPKM_OLDDSFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IdleState();
	void RunState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	void Moving(float speed, FVector dir);
	void BackStepState();
	void RushAttackState();
	void StingAttackState();
	void StingTwoAttackState();
	void SwingAttackState();
	void RangeAttackState();
	void RaiseAttackState();
	void TakeDownAttackState();
	void TestAttackState();
	void ReciveDamage(float value);
	float GiveDamage();
	int32 MyAttacktype;
	UPROPERTY(EditAnywhere, Category ="FSM")
	float currentTime=0;
	float idleDelayTime=2;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FSM")
	EEnemyState mState=EEnemyState::Idle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FSM")
	class APSH_CPlayer* Target;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	class APKM_OLDDS *Me;
	FVector direction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float distance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float RunRange = 3000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float MoveRange = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float BackRange = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float attackRange = 400.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float attackDelayTime = 2.0f;
	float BackStepSpeed;
	float StingSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 HP=10;
	UPROPERTY(EditAnywhere,Category="FSMDraw")
	bool bDebugRange=false;
	bool bStingdirCheck = false;
	bool bRushdirCheck=false;
	bool bRangeAttackHit = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bBackStepAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bRushAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bWalkAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bRunAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bRangeAttackAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bStingAttackAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bStingTwoAttackAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bSwingAttackAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bRaiseAttackAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bTakeDownAttackAnimCheck = false;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	bool bTestAttackAnimCheck = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	bool bTestAttackAnimPlayingEnd = false;

	float MovingSpeed=0;
	int32 ComboCount = 0;
	//연속공격 변수
	int32 SequenceAttack = 0;
};