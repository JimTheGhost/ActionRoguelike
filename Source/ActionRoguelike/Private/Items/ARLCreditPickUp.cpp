// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ARLCreditPickUp.h"

#include "Player/ARLPlayerState.h"

AARLCreditPickUp::AARLCreditPickUp()
{
	Value = 10;
}

void AARLCreditPickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (AARLPlayerState* PlayerState = Cast<AARLPlayerState>(InstigatorPawn->GetPlayerState()))
	{
		PlayerState->UpdateCredits(Value);
	}
	Super::Interact_Implementation(InstigatorPawn);
}
