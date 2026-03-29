// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARLPlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARLPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARLPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Currency")
	int32 Credits;
	
	public:
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
	int32 GetCredits() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
	bool UpdateCredits(int32 Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
	bool CanAfford(int32 Cost);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};
