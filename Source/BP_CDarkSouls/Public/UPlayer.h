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

	//������ ������

	UPROPERTY(EditAnywhere)
	class USpringArmComponent * compArm;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent * compCamera;
	
	UPROPERTY(EditAnywhere)
	class UMeshComponent * compMesh;

	UPROPERTY(EditAnywhere)
	FTimerHandle HoldTimerHandle;

	//������

	UPROPERTY(EditAnywhere) // ���� �޸��� �ִ���
	bool isRun = false;

	UPROPERTY(EditAnywhere) // ��ư�� ���ȴ���
	bool isPressed = false;

	UPROPERTY(EditAnywhere) // �齺���� ����ߴ���
	bool isBackstep=false;

	UPROPERTY(EditAnywhere)
	bool isPressedMovekey=false;

	UPROPERTY(EditAnywhere) // ���� �ð�
	float pressedTime = 0;

	UPROPERTY(EditAnywhere) // �� �ð��� ������ �׼��� �����
	float changeActionTime = 0.5f;

	UPROPERTY(EditAnywhere) // �ð��� ���� ����
	float curTime;
	
	UPROPERTY(EditAnywhere) // �齺�� ���� �ð�
	float backStepTime = 0.5;
	
	UPROPERTY(EditAnywhere) // �齺�� �Ÿ�
	float BackstepDest = 0;

	UPROPERTY(EditAnywhere) // ������ ��ġ
	FVector StartVector;
	UPROPERTY(EditAnywhere) // ������ ��ġ
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

	//���콺�� ���� ȸ��
	UFUNCTION()
	void Turn(float value);
	UFUNCTION()
	void LookUp(float value);

	void RollBackStepRun();

	void PressedSpacebar();
	void ReleasedSpacebar();
};
