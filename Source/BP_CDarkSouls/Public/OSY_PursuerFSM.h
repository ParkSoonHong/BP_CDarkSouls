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

	// 필요속성 : 대기시간,경과시간
	UPROPERTY(EditAnywhere, Category ="FSM")
	float idleDelayTime = 2;
	float currentTIme = 0;
	// 필요속성 : 타겟, 이동속도, 방향
	UPROPERTY(EditAnywhere, Category ="FSM")
	float speed= 500;
	// 나를 소유하고 있는 액터
	UPROPERTY()
	class AOSY_Pursuer *me;
	// 타겟 선언
	UPROPERTY(EditAnywhere)
	class AUPlayer *Target;

public:	 // 상태 함수
	void IdleState();
	void MoveState();
	void DashState();
	void AttackState();
	void DamageState();
	void DieState();

public: //Idle 속성
	// 필요속성 : 플레이어와의 거리, 대시거리, 무브거리,어택거리
	float DashDistance = 3000;
	float MoveDistance = 2000;
	float AttackDistance = 200;

		
};
