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
#include "Kismet/GameplayStatics.h"
#include "PKM_OLDDS.h"
#include "OSY_Pursuer.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "PSH_Shield.h"
#include "Particles/ParticleSystem.h"


// Sets default values
APSH_CPlayer::APSH_CPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	shield = CreateDefaultSubobject<UPSH_Shield>(TEXT("ShieldComponent"));
	
	shield->SetupAttachment(GetMesh(),TEXT("S_Shield"));
	compSword->SetupAttachment(GetMesh(),TEXT("S_Sword"));

	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSword(TEXT("/Script/Engine.StaticMesh'/Game/ParkSoonHong/Asset/Sword/sowrd.sowrd'"));
	if (tempSword.Succeeded())
	{
		compSword->SetStaticMesh(tempSword.Object);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Fallen_Knight/Mesh/SK_Fallen_Knight.SK_Fallen_Knight'"));

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
	GetMesh()->SetCollisionProfileName("Player");
	compSword->SetCollisionProfileName("PlayerWeapon");
	
	ConstructorHelpers::FClassFinder<UUserWidget>tempDieWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ParkSoonHong/WB_PSH_Die.WB_PSH_Die_C'"));
	if (tempDieWidget.Succeeded())
	{
		compDieWidget = tempDieWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget>tmepPlayMainWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BP_GamPlayMain.BP_GamPlayMain_C'"));
	if (tmepPlayMainWidget.Succeeded())
	{
		playMainWidget = tmepPlayMainWidget.Class;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempDieSound(TEXT("/Script/Engine.SoundWave'/Game/ParkSoonHong/sound/dark-souls-you-died-sound-effect.dark-souls-you-died-sound-effect'"));
	if (tempDieWidget.Succeeded())
	{
			dieSound = tempDieSound.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> tmepblood(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if (tmepblood.Succeeded())
	{
		blood = tmepblood.Object;
	}

	SworldCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollsion"));

	SworldCollision->SetupAttachment(compSword);
	SworldCollision->SetRelativeLocation(FVector(0,0,-50));
	SworldCollision->SetCapsuleSize(10,80);

	SworldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APSH_CPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	curStamina = maxStamina;
	anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APSH_CPlayer::OnComponentBeginOverlap);

	compSword->SetVisibility(false);
	shield->SetVisibility(false);
	
	CreateWidget(GetWorld(), playMainWidget)->AddToViewport();
	
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

	if (curTime > runTime)
	{
		curStamina -= 0.1;
		retunSpeed = runSpeed;  // 달리기로 바꾸기
		isRun = true;
	}

	if (isRun) // 달리는 상태면
	{
		Run();
	}

	if (isTimeOn)
	{
		curTime += DeltaTime;
	}

	if (isDie)
	{
		if (curTime >= 5)
		{
			curTime = 0;
			UGameplayStatics::OpenLevel(this,FName("Start"));
		}
	}
	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, retunSpeed, 2 * DeltaTime);

	if(!isDefense)
	{
	anim->Montage_JumpToSection(FName("defenceEnd"), anim->DefenseOnMontage);
	}

	if (isDamaged)
	{
		curHped -= 0.025;
		if (curHped <= curHp)
		{
			isDamaged = false;
		}
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

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APSH_CPlayer::Attack);
	PlayerInputComponent->BindAction("HardAttack", IE_Pressed, this, &APSH_CPlayer::HardAttack);

	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &APSH_CPlayer::Parry);

	PlayerInputComponent->BindAction("Defense", IE_Pressed, this, &APSH_CPlayer::DefenseOn);
	PlayerInputComponent->BindAction("Defense", IE_Released, this, &APSH_CPlayer::DefenseOff);

	PlayerInputComponent->BindAction("RifeTime", IE_Pressed, this, &APSH_CPlayer::healing);

	PlayerInputComponent->BindAction("WeaponChange",IE_Pressed, this,&APSH_CPlayer::changeWeapon);
}


void APSH_CPlayer::MoveForward(float Val)
{
	if (isMoving)
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
	if(isMoving)
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
	
}

void APSH_CPlayer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapOldDs = Cast<APKM_OLDDS>(UGameplayStatics::GetActorOfClass(GetWorld(), APKM_OLDDS::StaticClass()));
	tagetPursuer = Cast<AOSY_Pursuer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOSY_Pursuer::StaticClass()));

	if (!isDefenseTime)
	{
		if (OverlapOldDs != nullptr)
		{
			OverlapOldDs->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("Overlap"));
			Damaged(1);
			UE_LOG(LogTemp, Warning, TEXT("%d"), curHp);
		}

		if (tagetPursuer != nullptr)
		{
			tagetPursuer->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("Overlap"));
			Damaged(1);
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),blood,GetActorLocation(),FRotator::ZeroRotator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DefenseHit"));
		FVector NewPosition = GetActorLocation() - GetActorForwardVector() * MovePower;
		FVector dir = FMath::Lerp(GetActorLocation(),GetActorForwardVector()*MovePower,0.05f*GetWorld()->DeltaTimeSeconds);
		SetActorLocation(dir);
	}

}

//PKMWrite

void APSH_CPlayer::Attack()
{
	if(isAttackTime) // 공격 할수 있니?
	{ 	
			anim->PlayAttackAnimation();
			comboCount++;
		
		if (comboCount > 1) // 콤보가 트루야?
		{
				anim->PlayAttackAnimation2();
				comboCount = 0;
		}
		
	}

}

void APSH_CPlayer::HardAttack()
{
	if(isAttack)
	{ 
	anim->PlayHardAttackAnimation();
	}
}

void APSH_CPlayer::Parry()
{
	if(isDefense)
	{ 
	anim->PlayParryAnimation();
	UE_LOG(LogTemp, Warning, TEXT("Parry"));
	}
}

void APSH_CPlayer::DefenseOn() // 다른 행동이 실행 되면 이전 몽타주 
{
	UE_LOG(LogTemp,Warning,TEXT("Defense"));
	if(isDefense)
	{ 
		anim->PlayDefenseOnAnimation();
		isDefenseTime = true;
	}
}

void APSH_CPlayer::DefenseOff()
{
	if (isDefense)
	{
		anim->Montage_JumpToSection(FName("defenceEnd"), anim->DefenseOnMontage);
		isDefenseTime = false;
	}
}

void APSH_CPlayer::healing()
{
	if (isHearing)
	{
		anim->PlayHealingAnimation();
		if (curHp >= 10)
		{
			curHp = 10;
		}
		else
		{
			curHp += 5;
		}
	}
}


void APSH_CPlayer::Damaged(int value)
{
// 	if (curHp - value > 0)
// 	{
// 		curHp -= value;
// 		anim->PlayDamgedAnimation();
// 		UE_LOG(LogTemp, Warning, TEXT("damage&d"),curHp);
// 	}
// 	else
	switch (value) // value 가 데미지. 데미지만큼 깍아야하는데....
	{
	case 1:
		anim->PlayDamgedAnimation();
		curHp -= value;
		isDamaged = true;
		UE_LOG(LogTemp, Warning, TEXT("hit1"));
	break;
	case 2:
		anim->PlayDamgedAnimation2();
		curHp -= value;
		isDamaged = true;
		UE_LOG(LogTemp, Warning, TEXT("hit2"));
	break;
	case 3:
		anim->PlayDamgedAnimation3();
		curHp -= value;
		isDamaged = true;
		UE_LOG(LogTemp, Warning, TEXT("hit3"));
	default:
		break;
	}
	
	if(curHp<=0)
	{
		Die();
		isTimeOn = true;
		isDie = true;
	}

}

void APSH_CPlayer::Die()
{
	CreateWidget(GetWorld(),compDieWidget)->AddToViewport();
	UGameplayStatics::PlaySound2D(GetWorld(),dieSound);
	anim->PlayDeadAnimation();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void APSH_CPlayer::changeWeapon()
{
	// 트루이면 무기 들고 아니면 무기 안듬.
	if(anim->isChange) // 만약 바꿀게 가 들어오면
	{ 
		anim->PlayEquipAnimation();
		anim->isChange = false;
	}
	else
	{
		anim->PlayUnEquipAnimation();
		anim->isChange = true;
	}
}

void APSH_CPlayer::PressedSpacebar()
{
	isTimeOn = true;
	if (isPressedForwardMovekey || isPressedRightMovekey) //입력이 있을때
	{

	}
	else // 방향키 입력이 없을때
	{
		if (isBackStep) // is backstep이 true이면 백스텝을 해라.
		{
			curStamina -= 20;
			BackStep();
			isBackStep = false;
		}
	}
	
}

void APSH_CPlayer::ReleasedSpacebar()
{
	if (isPressedForwardMovekey || isPressedRightMovekey) // 입력이 있을때
	{	
		
		if (curTime < runTime)
		{	
			if (isRoll)
			{
				curStamina -= 30;
				Roll();
				isRoll = false;
			}

		}
	}
	curTime = 0;
	isRun = false;
	isRest = true;
	isTimeOn = false;
	retunSpeed = walkSpeed;
}

void APSH_CPlayer::steminaOring()
{
	//스테미나가 0인 상태 걷기, 방어를 제외한 모든 활동을 정지한다
	retunSpeed = walkSpeed;
	isRun = false;
	isRest = true;
	isTimeOn = false;
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
}

void APSH_CPlayer::Roll()
{	
	anim->PlayRollAnimation();
}

void APSH_CPlayer::BackStep()
{
	anim->PlayBackStepAnimation();

}
