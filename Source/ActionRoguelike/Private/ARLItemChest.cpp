// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLItemChest.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AARLItemChest::AARLItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMeshComp;

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMeshComp->SetupAttachment(BaseMeshComp);

	TargetPitch = 110;
	
	bReplicates = true;
}

void AARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpen = !bLidOpen;
	OnRep_ToggleChest();
}

void AARLItemChest::OnActorLoaded_Implementation()
{
	OnRep_ToggleChest();
	LogOnScreen(this,"Attempting Load");
}

void AARLItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AARLItemChest, bLidOpen);
}
void AARLItemChest::OnRep_ToggleChest()
 {		
	float CurrentPitch = bLidOpen ? TargetPitch:0;
	LidMeshComp->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
 }
