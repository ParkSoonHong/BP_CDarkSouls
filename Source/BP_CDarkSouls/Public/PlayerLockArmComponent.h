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
	// 액터를 대상으로 삼을 수 있는 캐릭터로부터의 최대 거리
	UPROPERTY(EditDefaultsOnly,Category = "Lock On Camera")
		float MaxTagetLockDistance;
		
	UPROPERTY(EditDefaultsOnly,Category = "Lock On Camera")
		bool bUseSoftLock;

	// 디버그 눈으로 보기 켜고끄는거
	UPROPERTY(EditDefaultsOnly,Category = "Lock On Camera")
		bool bDrawDebug;
	
	// 잠금이 최근에 해제되었고 마우스가 여전히 높은 경우
		bool bSoftlockRequireReset;
	
	// 카메라가 현재 잠겨 있는 구성 요소
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
