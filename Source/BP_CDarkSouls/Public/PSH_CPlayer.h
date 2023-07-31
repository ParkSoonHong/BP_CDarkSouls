// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PSH_CPlayer.generated.h"

UCLASS()
class BP_CDARKSOULS_API APSH_CPlayer : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UPlayerLockArmComponent* CameraLockArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/* Target component for camera lock on system */
	class UDSTargetComponent* TargetComponent;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock On Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock On Camera")
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock On Camera")
	float LockonControlRotationRate;;

	/* Tolerance for a mouse movement to be considered an input to switch targets */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float TargetSwitchMouseDelta;

	/* Tolerance for a mouse movement to be considered an input to switch targets */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float TargetSwitchAnalogValue;

	/* True if player returned analog-stick to center after last target switch */
	bool bAnalogSettledSinceLastTargetSwitch;

	/* Cooldown time before another target switch can occur, used to make target switching more controllable */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float TargetSwitchMinDelaySeconds;

	/* Time that last target switch occurred */
	UPROPERTY(BlueprintReadOnly, Category = "Lock On Camera")
	float LastTargetSwitchTime;

	/* Tolerance for a mouse movement to be considered an input to break target lock */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float BreakLockMouseDelta;

	/* Time to wait after breaking lock with mouse movement before player gets control of the camera back.  Prevents over scrolling camera after breaking lock. */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float BrokeLockAimingCooldown;

	/* Time that player broke camera lock at */
	float BrokeLockTime;
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	bool TagetOn = false;

public:
	// Sets default values for this character's properties
	APSH_CPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Setings")
	float walkSpeed = 500;
	// 뛰기 속도
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Setings")
	float runSpeed = 800;

	// 다시 바뀔 속도
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Setings")
	float retunSpeed=	500;

	UPROPERTY(EditAnywhere,Category = "Setings")
	bool isRun=false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "status")
	float maxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "status")
	float curStamina = 100;

	UPROPERTY(EditAnywhere,Category = "status")
	bool isRest = false;	// 스테미나 회복시간
	UPROPERTY(EditAnywhere,Category = "status")
	bool isOring = false;	// 스테미나 회복시간

	UPROPERTY(EditAnywhere,Category = "Time") // 달리기 시작 타임
	float runTime = 0.5f; 

	UPROPERTY(EditAnywhere,Category = "Time")
	float curTime = 0;

	UPROPERTY(EditAnywhere,Category = "Time")
	bool isTimeOn = false;

	UPROPERTY(EditAnywhere,Category = "status")
	bool isRoll = true;	

	UPROPERTY(EditAnywhere)
	bool isBackStep = true;

	bool isPressedForwardMovekey = false;
	bool isPressedRightMovekey = false;

	UPROPERTY(EditAnywhere,Category = "Attack")
	bool iscombo = false;

	UPROPERTY(EditAnywhere)
	bool isAttack = true;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool isDie = false;

	UPROPERTY(EditAnywhere)
	float iscombotime = 1.0;

	UPROPERTY(EditAnywhere)
	int32 comboCount = 0;

	UPROPERTY(EditAnywhere, Category = "Attack")
		bool PlayingAttack = false; 

	UPROPERTY(EditAnywhere, Category = "Attack")
		bool isAttackTime = true;

	UPROPERTY(EditAnywhere, Category = "Attack")
		bool isDamaged = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> compDieWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> playMainWidget;

	UPROPERTY(EditAnywhere)
	class UUserWidget* mainwidget;

	UPROPERTY(EditAnywhere)
	class UPSH_Shield* shield;

	UPROPERTY(EditAnywhere)
	bool isMoving = true;

	UPROPERTY(EditAnywhere)
	bool isHearing = true;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent * SworldCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isDefense = false;


	UPROPERTY(EditAnywhere)
	float MovePower = 100.0f;

	UPROPERTY(EditAnywhere)
	float steminaPower = 0.1f;

	UPROPERTY(EditAnywhere)
	bool isSteminaOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isParry = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool isTagetOn = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool isTagetoff = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool isEquip = false;

	UPROPERTY(EditAnywhere)
	bool isFire = true;

	UPROPERTY(EditAnywhere)
	float RLValu = 0;

	UPROPERTY(EditAnywhere)
	float FBValu = 0;
public:
	/** Returns CameraBoom subobject **/
	UFUNCTION(BlueprintCallable, Category = "Lock On Camera")
	FORCEINLINE class UPlayerLockArmComponent* GetCameraBoom() const { return CameraLockArm; }
	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable, Category = "Lock On Camera")
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compSword;

	//PKMWrite

	UPROPERTY(EditAnywhere)
	class AOSY_Pursuer * tagetPursuer;
	UPROPERTY(EditAnywhere)
	class APKM_OLDDS * OverlapOldDs;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PlayerWeaponComp;
	UPROPERTY(EditAnywhere)
	class UPlayerAnim * anim;
	UPROPERTY(EditAnywhere)
	class USoundBase * dieSound;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* blood;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* DefenseParticle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> ExFire;
	UPROPERTY(EditAnywhere)
	class UDSTargetComponent* tagetForward;
	
	UPROPERTY(EditAnywhere)
	class USoundBase * defenseSound;
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);

	/* Handle horizontal mouse input */
	void Turn(float Val);
	/* Handle vertical mouse input */
	void LookUp(float Val);

	// 스페이스바가 눌렸을때
	void PressedSpacebar();

	// 스페이스바가 때졌을때
	void ReleasedSpacebar();

	void steminaOring();

	void restTime();

	void Run();

	void Roll();

	void BackStep();

	void Attack();

	void HardAttack();

	void Parry();

	void DefenseOn();
	void DefenseOff();

	void healing();

	void Die();

	void changeWeapon();

	void tagetOn(UDSTargetComponent* NewTargetComponent);

	void FireMageic();
public:
	UFUNCTION()
	void NukBack();



	/* Tick every frame */
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	float PKMCurrentTime;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float maxHp = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float  curHp = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float  curHped = 5;
	void Damaged(int value);
};
