// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PSH_Shield.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BP_CDARKSOULS_API UPSH_Shield : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
		UPSH_Shield();

		UPROPERTY()
		class APSH_CPlayer* me;
	
		UPROPERTY()
		class UParticleSystem* DefenseParticle;

public:
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};


