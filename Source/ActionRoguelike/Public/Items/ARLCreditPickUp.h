// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPickUpBase.h"
#include "ARLCreditPickUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLCreditPickUp : public AARLPickUpBase
{
	GENERATED_BODY()
	AARLCreditPickUp();
	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Credits")
	int32 Value;
	
	public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
