// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARLPickUpBase.generated.h"
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLPickUpBase : public AActor, public IARLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARLPickUpBase();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Attributes|Lifetime")
	float RespawnDelay;
	UPROPERTY(EditAnywhere, Category = "Attributes|Lifetime")
	bool bRespawnable;

	FTimerHandle RespawnTimerHandle;

	virtual void DisablePickup(bool bRespawn);
	virtual void EnablePickup();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn);
};
