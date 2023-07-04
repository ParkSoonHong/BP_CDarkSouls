// Fill out your copyright notice in the Description page of Project Settings.


#include "UPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RotationMatrix.h"

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

}

// Called when the game starts or when spawned
void AUPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//player tick
	//UE_LOG(LogTemp,Warning,TEXT("% s"),isPressedMovekey)
	if (isBackstep==true)
	{
		curTime+=DeltaTime; // 시간을 더한다.
		
		//백스텝
		FVector BackstepVector = GetActorForwardVector() * -1; // 캐릭터의 정면에 -1곱해서 뒷면을 기준으로 잡음
		BackstepDest += 0.5f;
		FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; // 추후에 백스텝 천천히 하는걸로 교정
		SetActorLocation(BackstepLocation);

		if(curTime>=backStepTime)
		{ 
		isBackstep =false;
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

	PlayerInputComponent->BindAction(TEXT("Roll/BackStep/Run"),IE_Pressed,this,&AUPlayer::RollBackStepRun);
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
		GetCharacterMovement()->MaxWalkSpeed = 1200;
	}

	if (!isBackstep )
	{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, value);
	}

	if (value == 0)
	{
		isPressedMovekey = false;
	}else if (value != 0)
	{ 
		isPressedMovekey = true;
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
	if(!isBackstep)
	{ 
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, value);
	}
	
	if (value == 0)
	{
		isPressedMovekey = false;
	}
	else if (value != 0)
	{
		isPressedMovekey = true;
	}
}

void AUPlayer::Turn(float value)
{
	AddControllerYawInput(value);

}

void AUPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AUPlayer::RollBackStepRun()
{
	

	//Roll/BackStep/Run
	//isRun = true;
	
	//if() 방향키가 눌리고 있지 않다면.
	if(isPressedMovekey==false)
	{ 
	isBackstep = true;
	curTime = 0;
	BackstepDest = 0;
	}
	else if(isPressedMovekey==true) //방향키가 눌리고 있다면
	{

	pressedTime += 0.1f; //시간을 0.1초씩 더함
	if (isPressed && pressedTime >= changeActionTime) // 스페이스바가 눌려있고 눌려있던 시간이 변호 시간보다 크거나 같다면 변환
	{
		// 방향키 + 스페이스바 길게 누르면 달리기
	}
	else 
	{ 
		// 방향키 + 스페이스바 짧게 누르면 구르기
	}
	}
}

void AUPlayer::PressedSpacebar()
{
	// 스페이스바를 누르고 있는지 판별
	isPressed = true;
	pressedTime = 0;
	GetWorldTimerManager().SetTimer(HoldTimerHandle,this,&AUPlayer::RollBackStepRun,0.1f);
	// 타이머 설정 0.1초 마다 RollBackStepRun 실행
}

void AUPlayer::ReleasedSpacebar()
{
	// 스페이스바를 땐것을 판별 
	isPressed = false;
	isRun = false;
	GetWorldTimerManager().ClearTimer(HoldTimerHandle);
}

