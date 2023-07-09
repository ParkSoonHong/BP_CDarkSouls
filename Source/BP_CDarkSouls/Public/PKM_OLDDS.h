// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PKM_OLDDS.generated.h"

UCLASS()
class BP_CDARKSOULS_API APKM_OLDDS : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APKM_OLDDS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* springArmComp;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* HitComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* spearComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UPKM_OLDDSFSM* FSM;
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};