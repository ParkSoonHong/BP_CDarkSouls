// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OSY_Pursuer.generated.h"

UCLASS()
class BP_CDARKSOULS_API AOSY_Pursuer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOSY_Pursuer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UOSY_PursuerFSM* FSM;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compShield;



};
