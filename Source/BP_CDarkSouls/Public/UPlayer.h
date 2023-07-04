// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UPlayer.generated.h"

UCLASS()
class BP_CDARKSOULS_API AUPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//포인터 변수들

	UPROPERTY(EditAnywhere)
	class USpringArmComponent * compArm;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent * compCamera;
	
	UPROPERTY(EditAnywhere)
	class UMeshComponent * compMesh;

	UPROPERTY(EditAnywhere)
	FTimerHandle HoldTimerHandle;

	//변수들

	UPROPERTY(EditAnywhere) // 현재 달리고 있는지
	bool isRun = false;

	UPROPERTY(EditAnywhere) // 버튼이 눌렸는지
	bool isPressed = false;

	UPROPERTY(EditAnywhere) // 백스텝을 사용했는지
	bool isBackstep=false;

	UPROPERTY(EditAnywhere)
	bool isPressedMovekey=false;

	UPROPERTY(EditAnywhere) // 눌린 시간
	float pressedTime = 0;

	UPROPERTY(EditAnywhere) // 이 시간이 지나면 액션이 변경됨
	float changeActionTime = 0.5f;

	UPROPERTY(EditAnywhere) // 시간을 담을 변수
	float curTime;
	
	UPROPERTY(EditAnywhere) // 백스탭 시전 시간
	float backStepTime = 0.5;
	
	UPROPERTY(EditAnywhere) // 백스텝 거리
	float BackstepDest = 0;

	UPROPERTY(EditAnywhere) // 시작할 위치
	FVector StartVector;
	UPROPERTY(EditAnywhere) // 도착할 위치
	FVector tagetVector;


	/*
	UPROPERTY(EditAnywhere)
	bool isRun = false;
	*/
public:
	
	UFUNCTION()
	void Horizeontal(float value);

	UFUNCTION()
	void Vertical(float value);

	//마우스에 따른 회전
	UFUNCTION()
	void Turn(float value);
	UFUNCTION()
	void LookUp(float value);

	void RollBackStepRun();

	void PressedSpacebar();
	void ReleasedSpacebar();
};
