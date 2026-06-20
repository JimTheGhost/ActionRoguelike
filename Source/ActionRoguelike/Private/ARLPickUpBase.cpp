// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPickUpBase.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AARLPickUpBase::AARLPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	RespawnDelay = 10.0f;
	bRespawnable = true;
	bIsActive = true;

	SetReplicates(true);
}

void AARLPickUpBase::DisablePickup(bool bRespawn)
{
	SetPickupState(false);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&AARLPickUpBase::EnablePickup,RespawnDelay);
}

void AARLPickUpBase::EnablePickup()
{
	SetPickupState(true);
}

void AARLPickUpBase::SetPickupState(bool bNewState)
{
	bIsActive = bNewState;
	OnRep_IsActive();
}

void AARLPickUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	IARLGameplayInterface::Interact_Implementation(InstigatorPawn);
	DisablePickup(bRespawnable);
}

FText AARLPickUpBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void AARLPickUpBase::OnRep_IsActive()
{
	BaseMesh->SetVisibility(bIsActive,true);
	SetActorEnableCollision(bIsActive);
}
	
void AARLPickUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AARLPickUpBase, bIsActive);
}
