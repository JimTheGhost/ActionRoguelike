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

	SetReplicates(true);
}

void AARLPickUpBase::DisablePickup(bool bRespawn)
{
	BaseMesh->SetVisibility(false);
	SetActorEnableCollision(false);
	if (bRespawn && HasAuthority())
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&AARLPickUpBase::EnablePickupMulticast,RespawnDelay);
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
	DisablePickupMulticast(bRespawnable);
}

void AARLPickUpBase::DisablePickupMulticast_Implementation(bool bRespawn)
{
	DisablePickup(bRespawn);
}

void AARLPickUpBase::EnablePickupMulticast_Implementation()
{
	EnablePickup();
}

