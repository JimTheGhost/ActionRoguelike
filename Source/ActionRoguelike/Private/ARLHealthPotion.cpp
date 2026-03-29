// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLHealthPotion.h"
#include "ARLAttributeComponent.h"
#include "GameFramework/GameSession.h"
#include "Player/ARLPlayerState.h"

AARLHealthPotion::AARLHealthPotion()
{
	PrimaryActorTick.bCanEverTick = true;

	RespawnDelay = 10.0f;
	HealAmount = 30.0f;
	CreditCost = 10;
}

void AARLHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	AARLPlayerState* PS = Cast<AARLPlayerState>(InstigatorPawn->GetPlayerState());
	if (PS)
	{
		if (!PS->CanAfford(CreditCost))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Cannot afford");
			return;
		}
	}
	
	UARLAttributeComponent* AC = UARLAttributeComponent::GetAttributes(InstigatorPawn);
	if (ensure(AC) && !AC->IsFullHealth())
	{
		if (AC->ApplyHealthChange(HealAmount, InstigatorPawn) && PS->UpdateCredits(-CreditCost))
		{
			Super::Interact_Implementation(InstigatorPawn);
		}
	}
}
