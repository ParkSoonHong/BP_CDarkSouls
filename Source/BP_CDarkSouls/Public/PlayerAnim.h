// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class BP_CDARKSOULS_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Settings")
	float speed = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Settings")
	float diretion = 0;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* BackStepMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* HardAttackMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* parryMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* ShildMontage;

	void PlayRollAnimation();
	void PlayBackStepAnimation();
	void PlayAttackAnimation();
	void PlayHardAttackAnimation();
	void PlayShildAnimation();
	void PlayParryAnimation();

public:
	UPlayerAnim();
};
