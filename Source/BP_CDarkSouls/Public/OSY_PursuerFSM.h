// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OSY_PursuerFSM.generated.h"

UENUM(BlueprintType)
enum class EEnmeyState :uint8
{
	Idle UMETA(DisplayName = "Idle State"),
	Move UMETA(DisplayName = "Move State"),
	Dash UMETA(DisplayName = "Dash State"),
	Attack UMETA(DisplayName = "Attack State"),
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
	// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�, ����
	UPROPERTY(EditAnywhere, Category ="FSM")
	float speed= 500;
	// ���� �����ϰ� �ִ� ����
	UPROPERTY()
	class AOSY_Pursuer *me;
	// Ÿ�� ����
	UPROPERTY(EditAnywhere)
	class AUPlayer *Target;

public:	 // ���� �Լ�
	void IdleState();
	void MoveState();
	void DashState();
	void AttackState();
	void DamageState();
	void DieState();

public: //Idle �Ӽ�
	// �ʿ�Ӽ� : �÷��̾���� �Ÿ�, ��ðŸ�, ����Ÿ�,���ðŸ�
	float DashDistance = 3000;
	float MoveDistance = 2000;
	float AttackDistance = 200;

		
};
