// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_Pursuer.h"
#include "OSY_PursuerFSM.h"
#include "Components/CapsuleComponent.h"
#include "PSH_CPlayer.h"

// Sets default values
AOSY_Pursuer::AOSY_Pursuer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ½ºÄÌ·¹Å»-----------------------------------------------------------
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/OhSeYoung/Beta/Asset/Model/With_Bone_Pursure.With_Bone_Pursure'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh((TempMesh.Object));
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// ¼Òµå----------------------------------------------------------

	compSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("compSword"));
	compSword->SetupAttachment(GetMesh(),TEXT("Sword"));
	compSword->SetCollisionProfileName(TEXT("OSYSword"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/OhSeYoung/Asset/The_Pursuer/PC_Computer_-_Dark_Souls_II_-_The_Pursuer/The_Pursuer/sword.sword'"));
	if (TempSwordMesh.Succeeded())
	{
		compSword->SetStaticMesh(TempSwordMesh.Object);
		compSword->SetRelativeScale3D(FVector(1));
	}
	// ½Çµå----------------------------------------------------------
	compShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("compShield"));
	compShield->SetupAttachment(GetMesh(),TEXT("Shield"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempShieldMesh(TEXT("/Script/Engine.StaticMesh'/Game/OhSeYoung/Asset/The_Pursuer/PC_Computer_-_Dark_Souls_II_-_The_Pursuer/The_Pursuer/shield.shield'"));
	if (TempShieldMesh.Succeeded())
	{
		compShield->SetStaticMesh(TempShieldMesh.Object);
		compShield->SetRelativeLocation(FVector(-4.99f, -0.47f, -77.90f));
		compShield->SetRelativeRotation(FRotator(3.09f, -22.38f, 2.58f));
		compShield->SetRelativeScale3D(FVector(1));
	}
	// FSM----------------------------------------------------------
	FSM = CreateDefaultSubobject<UOSY_PursuerFSM>(TEXT("FSM"));

	// Animation-----------------------------------------------
	ConstructorHelpers::FClassFinder<UAnimInstance>tempclass(TEXT("//Script/Engine.AnimBlueprint'/Game/OhSeYoung/Blueprints/ABP_Pursuer.ABP_Pursuer_C'"));
	if (tempclass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempclass.Class);
	}

	HitComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitComp"));
	HitComp->SetupAttachment(RootComponent);
	HitComp->SetWorldScale3D(FVector(3, 3, 3));
	HitComp->SetCollisionProfileName(TEXT("Pursuer"));
	GetMesh()->SetCollisionProfileName(TEXT("Pursuer"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitComp->OnComponentBeginOverlap.AddDynamic(this, &AOSY_Pursuer::OnComponentBeginOverlap);

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

void AOSY_Pursuer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (FSM!=nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Good"));
		if (FSM->Target->PlayingAttack)
		{
			UE_LOG(LogTemp, Log, TEXT("Player->Hit"));
			FSM->ReciveDamage(1);
			FSM->Target->compSword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("BUG"));
	}
}

