// Fill out your copyright notice in the Description page of Project Settings.


#include "UPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RotationMatrix.h"
#include "Kismet/GameplayStatics.h"
#include "PKM_OLDDS.h"
#include "OSY_Pursuer.h"
#include "GameFramework/Character.h"

// Sets default values
AUPlayer::AUPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, 00, 0));
	}

	compArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	compArm->SetupAttachment(GetCapsuleComponent());
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	compCamera->SetupAttachment(compArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,4000,0);

	//PKM Write
	PlayerWeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	PlayerWeaponComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSMesh(TEXT("/Script/Engine.StaticMesh'/Game/ParkKyoungMin/Model/Spear/Myspear.Myspear'"));
	if (TempSMesh.Succeeded())
	{
		PlayerWeaponComp->SetStaticMesh(TempSMesh.Object);
		PlayerWeaponComp->SetRelativeLocationAndRotation(FVector(0, 0, 140), FRotator(0, 0, 0));
		PlayerWeaponComp->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	}
	PlayerWeaponComp->SetCollisionProfileName(TEXT("PlayerWeapon"));
	PlayerWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AUPlayer::BeginPlay()
{
	Super::BeginPlay();
	curHp = maxHp;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&AUPlayer::OnComponentBeginOverlap);
}

// Called every frame
void AUPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//player tick
	//UE_LOG(LogTemp,Warning,TEXT("% s"),isPressedMovekey)

	if(isBackstep)
	{			//백스텝
		curTime += DeltaTime; // 시간을 더한다.
		FVector BackstepVector = GetActorForwardVector(); // 캐릭터의 정면에 -1곱해서 뒷면을 기준으로 잡음
		//FVector BackstepLocation = FMath::Lerp(BackstepVector, BackstepTagetVector, 5*DeltaTime);
		//FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; // 추후에 백스텝 천천히 하는걸로 교정
		FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; 
		//10
		BackstepDest += -1;
		SetActorLocation(BackstepLocation);
	
		if (curTime >= backStepTime)
		{
			isNoWarlk = false;
			isBackstep = false;
// 			isPressedHorizontalMovekey = true;
// 			isPressedVerticalMovekey = true;

			curTime = 0;
			BackstepDest = -0; 
			
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"),BackstepDest);
		
	}

	if (isRoll)
	{			//구르기
		curTime += DeltaTime; // 시간을 더한다.
		FVector RollVector = GetActorForwardVector(); // 캐릭터의 정면에 -1곱해서 뒷면을 기준으로 잡음
		//FVector BackstepLocation = FMath::Lerp(BackstepVector, BackstepTagetVector, 5*DeltaTime);
		//FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; // 추후에 백스텝 천천히 하는걸로 교정
		FVector RollLocation = GetActorLocation() + RollVector * RollDest;
		//10
		RollDest += 1;
		SetActorLocation(RollLocation);
		UE_LOG(LogTemp, Warning, TEXT("Roll"));

		if (curTime >= RollTime)
		{
			isRoll = false;
			isNoWarlk = false;
			curTime = 0;
			RollDest = 0;
		
		}
	}

	if (isPressed)
	{
		pressedTime += DeltaTime;		
		RollBackStepRun(pressedTime);
	}

	if (setTagetLook)
	{
		UpdateCamer();
	}
	
	//PKMWtire
	if (PlayingAttack)
	{
		PKMCurrentTime += DeltaTime;
		if (PKMCurrentTime<0.3)
		{
			PlayerWeaponComp->SetRelativeRotation(FRotator(0, 360 * (PKMCurrentTime / 0.3), 0));
		}
		else
		{
			PlayerWeaponComp->SetRelativeRotation(FRotator(0, 0, 0));
			PlayingAttack = false;
			PKMCurrentTime = 0;
			PlayerWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// Called to bind functionality to input
void AUPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AUPlayer::Horizeontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AUPlayer::Vertical);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AUPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AUPlayer::LookUp);

	PlayerInputComponent->BindAction(TEXT("Roll/BackStep/Run"),IE_Pressed,this,&AUPlayer::PressedSpacebar); // 눌렀을때
	PlayerInputComponent->BindAction(TEXT("Roll/BackStep/Run"),IE_Released,this,&AUPlayer::ReleasedSpacebar);//땠을때

	PlayerInputComponent->BindAction(TEXT("TagetLook"),IE_Pressed,this,&AUPlayer::TagetLook);// 타겟 지정 눌렀을때

	//PKM Write
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AUPlayer::Attack);//땠을때
}

void AUPlayer::Horizeontal(float value)
{
	//FVector Dir = UKismetMathLibrary::GetRightVector(FRotator(0.0f,GetControlRotation().Yaw,0.0f));
	if(isRun == false)
	{ 
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 12000;
	}

	if (!isNoWarlk)
	{
			FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::Y);
			AddMovementInput(direction, value);
	}
	
	if (value == 0)
	{
		isPressedHorizontalMovekey = false;
		//UE_LOG(LogTemp, Warning, TEXT("No"));
	}else if (value != 0)
	{ 
		isPressedHorizontalMovekey = true;
		
		//UE_LOG(LogTemp,Warning,TEXT("On"));
	}
	
	
}

void AUPlayer::Vertical(float value)
{
	//FVector Dir = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	
	if (isRun == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200;
	}
	if(!isNoWarlk)
	{ 
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, value);
	}
	
	
	if (value == 0)
	{
		isPressedVerticalMovekey = false;
		//UE_LOG(LogTemp, Warning, TEXT("No2"));
	}
	else if (value != 0)
	{
		isPressedVerticalMovekey = true;
		
		//UE_LOG(LogTemp, Warning, TEXT("On2"));
	}
}

void AUPlayer::Turn(float value)
{
	
	AddControllerYawInput(value);
	//UE_LOG(LogTemp, Warning, TEXT("Turn"));
	
}

void AUPlayer::LookUp(float value)
{
	if(setTagetLook == false)
	{ 
	AddControllerPitchInput(value);
	//UE_LOG(LogTemp, Warning, TEXT("LookUp"));
	}
}

void AUPlayer::RollBackStepRun(float Time)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), Time);
	if (isPressedHorizontalMovekey || isPressedVerticalMovekey)
	{
		
			if (Time >= changeActionTime)
			{
				Run();
				UE_LOG(LogTemp, Warning, TEXT("GOGO"));
				UE_LOG(LogTemp, Warning, TEXT("Time : , %f"),Time);
			}
			else 
			{	// 불값을 통해 롤이랑 
				isNoRun = true;
				Roll();
				UE_LOG(LogTemp, Warning, TEXT("Roll"));
				UE_LOG(LogTemp, Warning, TEXT("Time : , %f"), Time);
			}
	}
	
}

void AUPlayer::PressedSpacebar() // 버튼이 눌렸을때
{

	if (isPressedHorizontalMovekey == false && isPressedVerticalMovekey == false)
	{
		BackStep();
		UE_LOG(LogTemp, Warning, TEXT("Backstep"));
	}
	else
	{
		isPressed = true;
	}
	
}

void AUPlayer::ReleasedSpacebar() // 버튼이 눌리지 않을때
{
	pressedTime = 0;
	isPressed = false;
	
}

void AUPlayer::Roll() // 구르기
{
	
	isRoll = true;
	isRun = false;
	isNoWarlk = true;
	
}

void AUPlayer::BackStep() // 백스탭
{
	isBackstep = true;
	isNoWarlk = true;
	isNoRun =false;

}

void AUPlayer::Run() // 달리기
{
	isNoWarlk = false;
	isRun = true;
	isRoll = false;
}

void AUPlayer::TagetLook()
{
	// 한번 누르면 가장 가장 가까운 적을 지정해서 본다.
	// 한번 누른다.
	setTagetLook = true;
	tagetLookNum++;
	UE_LOG(LogTemp,Warning,TEXT("%d"),tagetLookNum);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	if (setTagetLook)
	{
		tagetOldDs = Cast<APKM_OLDDS>(UGameplayStatics::GetActorOfClass(GetWorld(),APKM_OLDDS::StaticClass()));
		tagetPursuer = Cast<AOSY_Pursuer>(UGameplayStatics::GetActorOfClass(GetWorld(),AOSY_Pursuer::StaticClass()));

	}

	if (tagetLookNum > 1)
	{
		setTagetLook = false;
		tagetLookNum = 0;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	// 적을 지정한다.
	// 카메라로 본다.
}


void AUPlayer::UpdateCamer()
{	
	if(tagetPursuer != nullptr)
	{ 
	FVector Direction = tagetPursuer->GetActorLocation() - GetActorLocation();
	FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),tagetPursuer->GetActorLocation());
	GetController()->SetControlRotation(PlayerRot);
	compCamera->SetRelativeRotation(PlayerRot);
	}
	if (tagetOldDs != nullptr)
	{
		FVector Direction = tagetOldDs->GetActorLocation() - GetActorLocation();
		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), tagetOldDs->GetActorLocation());
		GetController()->SetControlRotation(PlayerRot);
	}
}

void AUPlayer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	OverlapOldDs= Cast<APKM_OLDDS>(UGameplayStatics::GetActorOfClass(GetWorld(), APKM_OLDDS::StaticClass()));
	if (OverlapOldDs!=nullptr)
	{
		OverlapOldDs->spearComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		curHp -= damge;
		UE_LOG(LogTemp, Warning, TEXT("%d"), curHp);
		if (curHp <= 0)
		{
			Destroy();
		}
	}
	tagetPursuer = Cast<AOSY_Pursuer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOSY_Pursuer::StaticClass()));
	if (tagetPursuer != nullptr)
	{
		tagetPursuer->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		curHp -= damge;
		UE_LOG(LogTemp, Warning, TEXT("%d"), curHp);
		if (curHp <= 0)
		{
			Destroy();
		}
	}

	
}

void AUPlayer::Attack()
{
	if (PlayingAttack==false)
	{
		PlayerWeaponComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PKMCurrentTime = 0;
		PlayingAttack = true;
	}
}

