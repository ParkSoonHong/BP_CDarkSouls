// Fill out your copyright notice in the Description page of Project Settings.


#include "PSH_Shield.h"
#include "PSH_CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UPSH_Shield::UPSH_Shield()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempShield(TEXT("/Script/Engine.StaticMesh'/Game/ParkSoonHong/Asset/Shield/shild_mesh0.shild_mesh0'"));
	if (tempShield.Succeeded())
	{
		SetStaticMesh(tempShield.Object);
	}

}

