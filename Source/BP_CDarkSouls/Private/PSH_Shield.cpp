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


	OnComponentBeginOverlap.AddDynamic(this, &UPSH_Shield::OnComponentOverlap);

	
	// 뒤로 밀려나기
	me = Cast<APSH_CPlayer>(GetOwner());
}


void UPSH_Shield::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<APSH_CPlayer>(OtherActor);
	if (player)
	{
		return;
	}
	else
	{
		if (me != nullptr)
		{
		me->NukBack();
		}
	}
	UE_LOG(LogTemp,Warning,TEXT("%s"),*OtherActor->GetName());
	/*me->NukBack();*/
}
