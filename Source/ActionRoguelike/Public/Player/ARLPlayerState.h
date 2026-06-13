// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARLPlayerState.generated.h"

class UARLSaveGame;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARLPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARLPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", BlueprintReadWrite, Category = "Inventory|Currency")
	int32 Credits;
	
	public:
	UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
	bool UpdateCredits(int32 Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
	bool CanAfford(int32 Cost);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Save Game")
	void SavePlayerState(UARLSaveGame* SaveGameObject);
	UFUNCTION(BlueprintNativeEvent, Category = "Save Game")
	void LoadPlayerState(UARLSaveGame* SaveGameObject);
	
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);
	
	//UFUNCTION(NetMulticast, Unreliable)
	//void OnCreditsChangedMulticast(AARLPlayerState* PlayerState, int32 NewCredits, int32 Delta);
};
