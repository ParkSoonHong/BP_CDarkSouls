// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMoveActor.h"
#include "Components/BoxComponent.h"
#include "UPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelMoveActor::ALevelMoveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NextLavel"));
	RootComponent = compBox;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	compMesh->SetupAttachment(RootComponent);

	// ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh (TEXT(""));

	//compBox->SetCollisionProfileName()
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ALevelMoveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelMoveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelMoveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AUPlayer * player = Cast<AUPlayer>(OtherActor);
	if (player != nullptr)
	{
		UGameplayStatics::OpenLevel(this,transLevel);
	}
}
