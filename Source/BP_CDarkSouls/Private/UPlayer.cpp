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

	if(isBackstep)
	{			//�齺��
		curTime += DeltaTime; // �ð��� ���Ѵ�.
		FVector BackstepVector = GetActorForwardVector() * -1; // ĳ������ ���鿡 -1���ؼ� �޸��� �������� ����
		//FVector BackstepLocation = FMath::Lerp(BackstepVector, BackstepTagetVector, 5*DeltaTime);
		//FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; // ���Ŀ� �齺�� õõ�� �ϴ°ɷ� ����
		FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; 
		//10
		BackstepDest -= 2;
		SetActorLocation(BackstepLocation);
		if (curTime >= backStepTime)
		{
			isBackstep = false;
// 			isPressedHorizontalMovekey = true;
// 			isPressedVerticalMovekey = true;
			curTime = 0;
			BackstepDest = 50;
		}
	}

	if (isRoll)
	{			//������
		curTime += DeltaTime; // �ð��� ���Ѵ�.
		FVector RollVector = GetActorForwardVector(); // ĳ������ ���鿡 -1���ؼ� �޸��� �������� ����
		//FVector BackstepLocation = FMath::Lerp(BackstepVector, BackstepTagetVector, 5*DeltaTime);
		//FVector BackstepLocation = GetActorLocation() + BackstepVector * BackstepDest; // ���Ŀ� �齺�� õõ�� �ϴ°ɷ� ����
		FVector RollLocation = GetActorLocation() + RollVector * RollDest;
		//10
		RollDest += 2;
		SetActorLocation(RollLocation);
		if (curTime >= RollTime)
		{
			isRoll = false;
			curTime = 0;
			RollDest = 50;
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
		GetCharacterMovement()->MaxWalkSpeed = 12000;
	}

	if (!isBackstep )
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
	if(!isBackstep)
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

}

void AUPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AUPlayer::RollBackStepRun()
{
	
	//if() ����Ű�� ������ ���� �ʴٸ�.
	if(isPressedHorizontalMovekey==false&&isPressedVerticalMovekey==false)
	{ 
			BackStep();
			isRun = false;
	}
	else if(isPressedHorizontalMovekey||isPressedVerticalMovekey) //����Ű�� ������ �ִٸ�
	{
		UE_LOG(LogTemp, Warning, TEXT("curTimeOn"));
		pressedTime +=0.1;
		
		if (pressedTime >= changeActionTime) // �����̽��ٰ� �����ְ� �����ִ� �ð��� ��ȣ �ð����� ũ�ų� ���ٸ� ��ȯ
		{
			// ����Ű + �����̽��� ��� ������ �޸���
			Run();
		//	UE_LOG(LogTemp,Warning,TEXT("GOGO"));
		}
		else 
		{ 
			// ����Ű + �����̽��� ª�� ������ ������
			Roll();
			curTime =0;
		//	UE_LOG(LogTemp, Warning, TEXT("Roll"));
		}
	}
	

	
}

void AUPlayer::PressedSpacebar()
{
	
	/*
	isPressed = true;
	isRun =true;
	isBackstep =true;
	pressedTime = 0;
	GetWorldTimerManager().SetTimer(HoldTimerHandle,this,&AUPlayer::RollBackStepRun,0.1f);
	*/
	
}

void AUPlayer::Roll()
{
	isRoll = true;
}

void AUPlayer::BackStep()
{
	isBackstep = true;
}

void AUPlayer::Run()
{
	isRun = true;
}

void AUPlayer::ReleasedSpacebar()
{/*
	// �����̽��ٸ� ������ �Ǻ� 
	isPressed = false;
	isRun = false;
	GetWorldTimerManager().ClearTimer(HoldTimerHandle);*/
}

