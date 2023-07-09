// Fill out your copyright notice in the Description page of Project Settings.


#include "PKM_OLDDS.h"
#include "PKM_OLDDSFSM.h"
#include "Components/CapsuleComponent.h"
#include "UPlayer.h"
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
		GetMesh()->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSMesh(TEXT("/Script/Engine.StaticMesh'/Game/ParkKyoungMin/Model/Spear/Myspear.Myspear'"));
	spearComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpearMesh"));
	spearComp->SetupAttachment(GetMesh());
	//spearComp->SetupAttachment(GetMesh());
	if (TempSMesh.Succeeded())
	{
		spearComp->SetStaticMesh(TempSMesh.Object);
		spearComp->SetRelativeLocationAndRotation(FVector(0, 0, 320), FRotator(0, 0, 0));
		spearComp->SetWorldScale3D(FVector(1.5, 1.5, 1.5));
	}
	//spearComp->SetupAttachment(GetMesh());
	spearComp->SetCollisionProfileName(TEXT("PKMSpear"));
	FSM = CreateDefaultSubobject<UPKM_OLDDSFSM>(TEXT("FSM"));
	
	HitComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitComp"));
	HitComp->SetupAttachment(RootComponent);
	HitComp->SetWorldScale3D(FVector(3, 3, 3));
	HitComp->SetCollisionProfileName(TEXT("OLDDS"));
}

// Called when the game starts or when spawned
void APKM_OLDDS::BeginPlay()
{
	Super::BeginPlay();
	HitComp->OnComponentBeginOverlap.AddDynamic(this, &APKM_OLDDS::OnComponentBeginOverlap);

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


void APKM_OLDDS::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (FSM->Target->PlayingAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("Player->Hit"));
		FSM->ReciveDamage(1);
		FSM->Target->PlayerWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}