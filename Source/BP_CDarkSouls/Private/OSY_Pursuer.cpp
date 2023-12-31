// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_Pursuer.h"
#include "OSY_PursuerFSM.h"
#include "Components/CapsuleComponent.h"
#include "UPlayer.h"

// Sets default values
AOSY_Pursuer::AOSY_Pursuer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ���̷�Ż-----------------------------------------------------------
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/OhSeYoung/Asset/The_Pursuer/PC_Computer_-_Dark_Souls_II_-_The_Pursuer/The_Pursuer/untitled_R1.untitled_R1'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh((TempMesh.Object));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// �ҵ�----------------------------------------------------------
	compSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("compSword"));
	compSword->SetupAttachment(GetCapsuleComponent());
	compSword->SetRelativeLocation(FVector(0,90,20));
	compSword->SetRelativeRotation(FRotator(0,90,00));
	compSword->SetRelativeScale3D(FVector(1));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/OhSeYoung/Asset/The_Pursuer/PC_Computer_-_Dark_Souls_II_-_The_Pursuer/The_Pursuer/sword.sword'"));
	if (TempSwordMesh.Succeeded())
	{
		compSword->SetStaticMesh(TempSwordMesh.Object);
	}
	// �ǵ�----------------------------------------------------------
	compShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("compShield"));
	compShield->SetupAttachment(GetCapsuleComponent());
	compShield->SetRelativeLocation(FVector(0, -80, -30));
	compShield->SetRelativeRotation(FRotator(0, -90, 00));
	compShield->SetRelativeScale3D(FVector(1));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempShieldMesh(TEXT("/Script/Engine.StaticMesh'/Game/OhSeYoung/Asset/The_Pursuer/PC_Computer_-_Dark_Souls_II_-_The_Pursuer/The_Pursuer/shield.shield'"));
	if (TempShieldMesh.Succeeded())
	{
		compShield->SetStaticMesh(TempShieldMesh.Object);
	}
	// FSM----------------------------------------------------------
	FSM = CreateDefaultSubobject<UOSY_PursuerFSM>(TEXT("FSM"));

	HitComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitComp"));
	HitComp->SetupAttachment(RootComponent);
	HitComp->SetWorldScale3D(FVector(3, 3, 3));
	HitComp->SetCollisionProfileName(TEXT("Pursuer"));
}

// Called when the game starts or when spawned
void AOSY_Pursuer::BeginPlay()
{
	Super::BeginPlay();
	HitComp->OnComponentBeginOverlap.AddDynamic(this, &AOSY_Pursuer::OnComponentBeginOverlap);

	
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

void AOSY_Pursuer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (FSM!=nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Good"));
		if (FSM->Target->PlayingAttack)
		{
			FSM->ReciveDamage(1);
			FSM->Target->PlayerWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("BUG"));
	}
}

