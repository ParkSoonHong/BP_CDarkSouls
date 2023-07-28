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

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempDefenseParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Building_A.P_Destruction_Building_A'"));
	if (tempDefenseParticle.Succeeded())
	{
		DefenseParticle = tempDefenseParticle.Object;
	}
	/*me->SetupAttachment(me->GetMesh(), TEXT("S_Shield"));*/


	OnComponentBeginOverlap.AddDynamic(this, &UPSH_Shield::OnComponentOverlap);
	
}


void UPSH_Shield::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefenseParticle, OtherActor->GetActorLocation(), FRotator::ZeroRotator);
}
