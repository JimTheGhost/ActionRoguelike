// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPickUpBase.h"
#include "ARLPickUp_Action.generated.h"

class UARLAction;

UCLASS()
class ACTIONROGUELIKE_API AARLPickUp_Action : public AARLPickUpBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARLPickUp_Action();
	void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	TSubclassOf<UARLAction> ActionClass;
};
