// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPickUpBase.h"

// Sets default values
AARLPickUpBase::AARLPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	RespawnDelay = 10.0f;
	bRespawnable = true;

}

void AARLPickUpBase::DisablePickup(bool bRespawn)
{
	BaseMesh->SetVisibility(false);
	SetActorEnableCollision(false);
	if (bRespawn)
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&AARLPickUpBase::EnablePickup,RespawnDelay);
	}
}

void AARLPickUpBase::EnablePickup()
{
	BaseMesh->SetVisibility(true);
	SetActorEnableCollision(true);
}

void AARLPickUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	IARLGameplayInterface::Interact_Implementation(InstigatorPawn);
	DisablePickup(bRespawnable);
}

