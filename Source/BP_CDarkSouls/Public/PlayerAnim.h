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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Settings")
	bool isChange = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Settings")
	bool isTagetAnim = false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* RollMontage;
	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* RRollMontage;
	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* LRollMontage;
	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* BRollMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* BackStepMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* AttackMontage2;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* HardAttackMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* parryMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* DefenseOnMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* DamageMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* DamageMontage2;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* DamageMontage3;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* healingMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* deadMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* equipMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* unequipMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* StunMontage;

	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* FireMagicMontage;

	UPROPERTY()
	class APSH_CPlayer * Player;

	UPROPERTY(EditAnywhere)
	bool comboAttack = false;

	UPROPERTY(EditAnywhere)
	bool startAttack = true;

	UPROPERTY(EditAnywhere)
	bool AttackEnd = false;

	UPROPERTY(EditAnywhere)
	bool hardAttackEnd = false;

	UPROPERTY(EditAnywhere)
	bool isRoll = true;

	UPROPERTY(EditAnywhere)
	bool isBackStep = true;

	
	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_BackstetpTime();
	
	UFUNCTION()
	void AnimNotify_StartAttack();

	UFUNCTION()
	void AnimNotify_ComboAttack();

	UFUNCTION()
	void AnimNotify_RollEnd();

	UFUNCTION()
	void AnimNotify_endHardAttack();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_endHeling();
	
	UFUNCTION()
	void AnimNotify_movingOn();
	
	UFUNCTION()
	void AnimNotify_Equip();
	
	UFUNCTION()
	void AnimNotify_UnEquip();
	
	UFUNCTION()
	void AnimNotify_endEquip();

	UFUNCTION()
	void AnimNotify_ParryEnd();

	UFUNCTION()
	void AnimNotify_stunEnd();

	UFUNCTION()
	void AnimNotify_Magic();



	void PlayRollAnimation();
	void PlayRRollAnimation();
	void PlayLRollAnimation();
	void PlayBRollAnimation();
	void PlayBackStepAnimation();
	void PlayDamgedAnimation();
	void PlayDamgedAnimation2();
	void PlayDamgedAnimation3();
	void PlayAttackAnimation();
	void PlayAttackAnimation2();

	void PlayHardAttackAnimation();
	void PlayDefenseOnAnimation();
	
	void PlayParryAnimation();
	void PlayHealingAnimation();
	void PlayDeadAnimation();
	void PlayEquipAnimation();
	void PlayUnEquipAnimation();
	void PlayStunAnimation();
	void PlayFireMagicAnimation();


public:
	UPlayerAnim();
};
