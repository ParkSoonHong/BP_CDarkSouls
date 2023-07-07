// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelMoveActor.generated.h"

UCLASS()
class BP_CDARKSOULS_API ALevelMoveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelMoveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	FName transLevel;
	

	UPROPERTY(EditAnywhere)
	class UBoxComponent * compBox;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent * compMesh;

	void NotifyActorBeginOverlap(AActor* OtherActor);
};
