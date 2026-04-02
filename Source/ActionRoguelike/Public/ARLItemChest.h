// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARLItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLItemChest : public AActor, public IARLGameplayInterface
{
	GENERATED_BODY()
	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	AARLItemChest();

	UPROPERTY(EditAnywhere, Category = "Lid Mechanism")
	float TargetPitch;
protected:
	UPROPERTY(ReplicatedUsing="OnRep_ToggleChest", BlueprintReadOnly)
	bool bLidOpen;
	
	UFUNCTION()
	void OnRep_ToggleChest();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* LidMeshComp;
};
