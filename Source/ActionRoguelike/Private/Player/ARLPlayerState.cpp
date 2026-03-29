// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ARLPlayerState.h"

int32 AARLPlayerState::GetCredits() const
{
	return Credits;
}

bool AARLPlayerState::UpdateCredits(int32 Delta)
{
	int32 NewCredits = Credits + Delta;
	if (NewCredits < 0)
	{
		return false;
	}
	Credits = NewCredits;
	
	OnCreditsChanged.Broadcast(this, Credits, Delta);
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
