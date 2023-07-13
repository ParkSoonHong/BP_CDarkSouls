// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerLockArmComponent.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};


/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BP_CDARKSOULS_API UPlayerLockArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	// ���͸� ������� ���� �� �ִ� ĳ���ͷκ����� �ִ� �Ÿ�
	UPROPERTY(EditDefaultsOnly,Category = "Lock On Camera")
		float MaxTagetLockDistance;
		
	UPROPERTY(EditDefaultsOnly,Category = "Lock On Camera")
		bool bUseSoftLock;

	// ����� ������ ���� �Ѱ���°�
	UPROPERTY(EditDefaultsOnly,Category = "Lock On Camera")
		bool bDrawDebug;
	
	// ����� �ֱٿ� �����Ǿ��� ���콺�� ������ ���� ���
		bool bSoftlockRequireReset;
	
	// ī�޶� ���� ��� �ִ� ���� ���
	UPROPERTY(BlueprintReadOnly)
		class UDSTargetComponent* CameraTarget;

	UPROPERTY(EditAnywhere)
	class APSH_CPlayer * player;
	
	UPlayerLockArmComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void ToggleCameraLock();
	void ToggleSoftLock();
	void LockToTarget(UDSTargetComponent* NewTargetComponent);
	void BreakTargetLock();

	UFUNCTION()
	class UDSTargetComponent* GetLockTarget();

	void SwitchTarget(EDirection SwitchDirection);
	TArray<class UDSTargetComponent*> GetTargetComponents();

	/* True if the camera is currently locked to a target */
 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lock On Camera")
 		bool IsCameraLockedToTarget();
		
};
