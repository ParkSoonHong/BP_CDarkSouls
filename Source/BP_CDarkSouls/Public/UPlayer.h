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

	UPROPERTY(EditAnywhere)
	bool isRun = false;

	
	UPROPERTY(EditAnywhere)
	bool isPressed = false;

	UPROPERTY(EditAnywhere)
	float pressedTime = 0;

	UPROPERTY(EditAnywhere)
	float changeActionTime = 0.5f;

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
