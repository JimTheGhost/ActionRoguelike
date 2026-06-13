// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ARLPlayerState.h"

#include "ARLSaveGame.h"
#include "Net/UnrealNetwork.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

bool AARLPlayerState::UpdateCredits(int32 Delta)
{
	int32 NewCredits = Credits + Delta;
	if (NewCredits < 0)
	{
		return false;
	}
	Credits = NewCredits;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
	/*if (HasAuthority())
	{
		OnCreditsChangedMulticast(this, NewCredits, Delta);
	}*/
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

void AARLPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}


void AARLPlayerState::SavePlayerState_Implementation(UARLSaveGame* SaveGameObject)
{
	if (SaveGameObject)
	{
		SaveGameObject->Credits = Credits;
	}
}

void AARLPlayerState::LoadPlayerState_Implementation(UARLSaveGame* SaveGameObject)
{
	if (SaveGameObject)
	{
		Credits = SaveGameObject->Credits;
		OnCreditsChanged.Broadcast(this, Credits, 0);
	}
}

/*
void AARLPlayerState::OnCreditsChangedMulticast_Implementation(AARLPlayerState* PlayerState, int32 NewCredits,
                                                               int32 Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreditsChangedMulticast — Role: %d, NewCredits: %d"),
	(int32)GetLocalRole(), NewCredits);
	OnCreditsChanged.Broadcast(this, NewCredits, Delta);
}
*/

void AARLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AARLPlayerState, Credits);
}
