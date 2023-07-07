// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_Pursuer.h"
#include "OSY_PursuerFSM.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AOSY_Pursuer::AOSY_Pursuer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compSword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("compSword"));
	compSword->SetupAttachment(GetCapsuleComponent());
	compSword->SetRelativeLocation(FVector(0,90,60));
	compSword->SetRelativeRotation(FRotator(0,90,90));
	compSword->SetRelativeScale3D(FVector(0.28));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSwordMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/EditorMeshes/SkeletalMesh/DefaultSkeletalMesh.DefaultSkeletalMesh'"));
	if (TempSwordMesh.Succeeded())
	{
		compSword->SetSkeletalMesh(TempSwordMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh((TempMesh.Object));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	FSM = CreateDefaultSubobject<UOSY_PursuerFSM>(TEXT("FSM"));

}

// Called when the game starts or when spawned
void AOSY_Pursuer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOSY_Pursuer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOSY_Pursuer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

