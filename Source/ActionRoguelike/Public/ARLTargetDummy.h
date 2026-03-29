// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLTargetDummy.generated.h"

class UStaticMeshComponent;
class UARLAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API AARLTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARLTargetDummy();

protected:
	// Components
	UPROPERTY(VisibleAnywhere)
	UARLAttributeComponent* AttributeComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMeshComp;
	// Initialization Functions
	virtual void PostInitializeComponents() override;
	// Bind Functions for Delegates
	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);

};
