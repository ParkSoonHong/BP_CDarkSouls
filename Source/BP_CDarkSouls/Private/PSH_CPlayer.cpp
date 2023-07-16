// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "DSTargetComponent.h"
#include "PlayerLockArmComponent.h"
#include "PlayerAnim.h"

// Sets default values
APSH_CPlayer::APSH_CPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParkSoonHong/Animation/Falling_To_Roll.Falling_To_Roll'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	LockonControlRotationRate = 10.f;
	TargetSwitchMouseDelta = 3.f;
	TargetSwitchAnalogValue = .8f;
	TargetSwitchMinDelaySeconds = .5f;
	BreakLockMouseDelta = 10.f;
	BrokeLockAimingCooldown = .5f;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 2000.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraLockArm = CreateDefaultSubobject<UPlayerLockArmComponent>(TEXT("CameraLockArm"));
	CameraLockArm->SetupAttachment(RootComponent);
	CameraLockArm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraLockArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create target component
	TargetComponent = CreateDefaultSubobject<UDSTargetComponent>(TEXT("TargetComponent"));
	TargetComponent->SetupAttachment(GetRootComponent());

	ConstructorHelpers::FClassFinder<UPlayerAnim>tempEnemy(TEXT("/Script/Engine.BlendSpace'/Game/ParkSoonHong/Ani/BLend_PSHPlayer.BLend_PSHPlayer_C'"));
	if (tempEnemy.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempEnemy.Class);
	}
}

// Called when the game starts or when spawned
void APSH_CPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	curStamina = maxStamina;

	anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame

void APSH_CPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (curStamina <= 0) // 스테미나가 0일때
	{	
		curStamina = 0;
		steminaOring();
	}

	if (isRest)
	{
		restTime();
	}

	if (isRun) // 달리는 상태면
	{
		Run();
	}

	if (isRoll)
	{
		Roll();
	}

	if (isTimeOn)
	{
		curTime += DeltaTime;
	}
}

// Called to bind functionality to input
void APSH_CPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &APSH_CPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Horizontal", this, &APSH_CPlayer::MoveRight);

	// Handle mouse aiming input
	PlayerInputComponent->BindAxis("Turn", this, &APSH_CPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APSH_CPlayer::LookUp);

	PlayerInputComponent->BindAction(TEXT("Roll/BackStep/Run"), IE_Pressed, this, &APSH_CPlayer::PressedSpacebar); // 눌렀을때
	PlayerInputComponent->BindAction(TEXT("Roll/BackStep/Run"), IE_Released, this, &APSH_CPlayer::ReleasedSpacebar);//땠을때

	// Action inputs
	PlayerInputComponent->BindAction("TagetLook", IE_Pressed, CameraLockArm, &UPlayerLockArmComponent::ToggleCameraLock);
	PlayerInputComponent->BindAction("TagetLook", IE_Pressed, CameraLockArm, &UPlayerLockArmComponent::ToggleSoftLock);
}


void APSH_CPlayer::MoveForward(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = CameraLockArm->CameraTarget == nullptr ? Controller->GetControlRotation() : (CameraLockArm->CameraTarget->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}

	if (Val != 0) // 움직이고 있다.
	{
		isPressedForwardMovekey = true;
	}
	else if (Val == 0) // 움직이지 않는다.
	{
		isPressedForwardMovekey = false;
	}
}

void APSH_CPlayer::MoveRight(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = CameraLockArm->CameraTarget == nullptr ? Controller->GetControlRotation() : (CameraLockArm->CameraTarget->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}

	if (Val != 0) // 움직이고 있다.
	{
		isPressedRightMovekey = true;
	}
	else if (Val == 0) // 움직이지 않는다.
	{
		isPressedRightMovekey = false;
	}

}

void APSH_CPlayer::Turn(float Val)
{
if(!TagetOn)
{ 
	float TimeSinceLastTargetSwitch = GetWorld()->GetRealTimeSeconds() - LastTargetSwitchTime;

	if (CameraLockArm->IsCameraLockedToTarget())
	{
		// Should break soft-lock?
		if (CameraLockArm->bUseSoftLock && FMath::Abs(Val) > BreakLockMouseDelta)
		{
			CameraLockArm->BreakTargetLock();
			BrokeLockTime = GetWorld()->GetRealTimeSeconds();
			CameraLockArm->bSoftlockRequireReset = true;
		}
		// Should try switch target?
		else if (FMath::Abs(Val) > TargetSwitchMouseDelta
			&& TimeSinceLastTargetSwitch > TargetSwitchMinDelaySeconds)	// Prevent switching multiple times using a single movement
		{
			if (Val < 0)
				CameraLockArm->SwitchTarget(EDirection::Left);
			else
				CameraLockArm->SwitchTarget(EDirection::Right);

			LastTargetSwitchTime = GetWorld()->GetRealTimeSeconds();
		}
	}
	else
	{
		// If camera lock was recently broken by a large mouse delta, allow a cooldown time to prevent erratic camera movement
		bool bRecentlyBrokeLock = (GetWorld()->GetRealTimeSeconds() - BrokeLockTime) < BrokeLockAimingCooldown;
		if (!bRecentlyBrokeLock)
			AddControllerYawInput(Val);
	}
}
}

void APSH_CPlayer::LookUp(float Val)
{
	if(!TagetOn)
	{ 
		if (!CameraLockArm->IsCameraLockedToTarget())
			AddControllerPitchInput(Val);
	}
}




void APSH_CPlayer::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (CameraLockArm->IsCameraLockedToTarget())
	{
		// Vector from player to target
		FVector TargetVect = CameraLockArm->CameraTarget->GetComponentLocation() - CameraLockArm->GetComponentLocation();
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, LockonControlRotationRate);

		// Update control rotation to face target
		GetController()->SetControlRotation(NewRot);
	}
	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, retunSpeed, 2 * DeltaTime);

}

//PKMWrite

void APSH_CPlayer::Attack()
{
	if (PlayingAttack == false)
	{
		PlayerWeaponComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PKMCurrentTime = 0;
		PlayingAttack = true;
	}
}

void APSH_CPlayer::Damaged(float value)
{
	if (curHp - value > 0)
	{
		curHp -= value;
		UE_LOG(LogTemp, Warning, TEXT("%d"), curHp);
	}
	else
	{
		curHp = 0;
		Destroy();
	}
}
void APSH_CPlayer::PressedSpacebar()
{
	isTimeOn = true;
	if (isPressedForwardMovekey || isPressedRightMovekey)
	{
		if (curTime >= runTime)
		{
			retunSpeed = runSpeed;  // 달리기로 바꾸기
			isRun = true;
		}
		
	}
	else
	{
		BackStep();
	}
	
}

void APSH_CPlayer::ReleasedSpacebar()
{
	if (isPressedForwardMovekey || isPressedRightMovekey)
	{
	Roll();
	}

	isRun = false;
	isRest = true;
	retunSpeed = walkSpeed;
}

void APSH_CPlayer::steminaOring()
{
	//스테미나가 0인 상태 걷기, 방어를 제외한 모든 활동을 정지한다
	retunSpeed = walkSpeed;
	isRun = false;
	isRest = true;
}

void APSH_CPlayer::restTime()
{
	curStamina += 30 * GetWorld()->DeltaTimeSeconds;
	if (curStamina >= maxStamina)
	{
		curStamina = 100;
		isRest = false;
	}
}

void APSH_CPlayer::Run()
{
	isRest = false;
	curStamina -= 25 * GetWorld()->DeltaTimeSeconds;
}

void APSH_CPlayer::Roll()
{	
	anim->PlayRollAnimation();
	curStamina -= 30;
}

void APSH_CPlayer::BackStep()
{
	anim->PlayBackStepAnimation();
	curStamina -= 20;
}
