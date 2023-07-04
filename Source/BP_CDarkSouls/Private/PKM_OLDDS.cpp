// Fill out your copyright notice in the Description page of Project Settings.


#include "PKM_OLDDS.h"
#include "PKM_OLDDSFSM.h"

// Sets default values
APKM_OLDDS::APKM_OLDDS()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParkKyoungMin/Model/MyOLDDS.MyOLDDS'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	FSM = CreateDefaultSubobject<UPKM_OLDDSFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void APKM_OLDDS::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APKM_OLDDS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APKM_OLDDS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

