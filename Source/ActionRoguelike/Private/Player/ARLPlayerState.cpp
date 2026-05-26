// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ARLPlayerState.h"

#include "Net/UnrealNetwork.h"

bool AARLPlayerState::UpdateCredits(int32 Delta)
{
	int32 NewCredits = Credits + Delta;
	if (NewCredits < 0)
	{
		return false;
	}
	Credits = NewCredits;
	
	if (HasAuthority())
	{
		OnCreditsChangedMulticast(this, NewCredits, Delta);
	}
	return true;
}

bool AARLPlayerState::CanAfford(int32 Cost)
{
	if (Credits >= Cost)
	{
		return true;
	}
	return false;
}

void AARLPlayerState::OnCreditsChangedMulticast_Implementation(AARLPlayerState* PlayerState, int32 NewCredits,
	int32 Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreditsChangedMulticast — Role: %d, NewCredits: %d"),
	(int32)GetLocalRole(), NewCredits);
	OnCreditsChanged.Broadcast(this, NewCredits, Delta);
}

void AARLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AARLPlayerState, Credits);
}
