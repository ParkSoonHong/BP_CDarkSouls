// Fill out your copyright notice in the Description page of Project Settings.


#include "OSY_BPSword.h"

// Sets default values
AOSY_BPSword::AOSY_BPSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SwordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>Swordtemp(TEXT("/Script/Engine.StaticMesh'/Game/OhSeYoung/Asset/The_Pursuer/PC_Computer_-_Dark_Souls_II_-_The_Pursuer/The_Pursuer/sword.sword'"));
	if (Swordtemp.Succeeded())
	{
		SwordComp->SetStaticMesh(Swordtemp.Object);
	}
}

// Called when the game starts or when spawned
void AOSY_BPSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOSY_BPSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

