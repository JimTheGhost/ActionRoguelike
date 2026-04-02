// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLInteractionComponent.generated.h"


class UARLWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractRange;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel;
	
	void PrimaryInteract();
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();
	
	UPROPERTY()
	AActor* FocusedActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARLWorldUserWidget> InteractWidgetClass;
	UPROPERTY()
	UARLWorldUserWidget* InteractWidget;
	
public:	
	// Sets default values for this component's properties
	UARLInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
