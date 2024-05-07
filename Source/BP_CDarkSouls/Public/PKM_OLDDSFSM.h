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
	UFUNCTION(BlueprintCallable)
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
	float BackRange = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float attackRange = 400.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	float attackDelayTime = 2.0f;
	float BackStepSpeed;
	float StingSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 OLDDSHP=10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	int32 OLDDSMAXHP=10;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FSM")
	bool bDieAnimCheck=false;
	
	int32 TESTRand = 1;
	float MovingSpeed=0;
	int32 ComboCount = 0;
	//연속공격 변수
	int32 SequenceAttack = 0;

	bool bRangeAttackEffect=false;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystem *RushAttackFactory;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystemComponent *RushAttackEffect;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystem *RangeAttackBallFactory;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystemComponent *RangeBallEffect;
   
	UPROPERTY(EditDefaultsOnly,Category="Effect")
	class UParticleSystem *RangeAttackFactory;
   
   UPROPERTY(EditDefaultsOnly,Category="Effect")
   class UParticleSystemComponent *RangeAttackEffect;

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

	int32 High = 0;
	FVector RangingLoc;
	FVector RangeStartLoc;



	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* RushChargeSound;
	
	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* RushSound;
	bool bRushSound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* RangeChargeSound;
	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* RangeSound;
	bool bRangeSound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* SwingSound;
	bool bSwingSound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* BackStepSound;
	bool bBackStepSound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* DamageSound;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* DownKneeSound;
	bool bDownKneeSound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* DeadSound;
	bool bDeadSound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* StingSound;
	bool bStingSound = false;
	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* Sting2Sound;
	bool bSting2Sound = false;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* MapSound;

	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class UAudioComponent* MapAudio;
	UPROPERTY(EditDefaultsOnly,Category="Sound")
	class USoundBase* DieBloodSound;

	int32 TestRandNum=1;
	bool bFirstDash = false;
	int32 BackRand;
	bool bBackStepRand=false;
};