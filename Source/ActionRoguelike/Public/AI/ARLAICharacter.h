// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLAICharacter.generated.h"

class UARLWorldUserWidget;
class UARLAttributeComponent;
class UARLActionComponent;
class UPawnSensingComponent;
class UUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLAICharacter();
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParam;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLAttributeComponent* AttributeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DetectionWidgetClass;

	UPROPERTY()
	UARLWorldUserWidget* ActiveHealthBar;
	UPROPERTY()
	UARLWorldUserWidget* ActiveDetectionWidget;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	virtual void PostInitializeComponents() override;

	virtual void SetTargetActor(AActor* TargetActor);
};
