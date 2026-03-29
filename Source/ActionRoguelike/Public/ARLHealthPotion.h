// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPickUpBase.h"
#include "ARLHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLHealthPotion : public AARLPickUpBase
{
	GENERATED_BODY()

public:
	AARLHealthPotion();
	//called with interact message
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float HealAmount;
	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 CreditCost;
};
