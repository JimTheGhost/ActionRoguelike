// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAbilityData.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ARLCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInputActionConfig;
class UARLInteractionComponent;
class UARLAttributeComponent;
class UARLActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLCharacter();

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);
protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UARLInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLAttributeComponent* AttributeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLActionComponent* ActionComp;
	
	//Input Variables
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="EnhancedInput")
	UInputMappingContext* PlayerContext;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="EnhancedInput")
	UInputActionConfig* InputActionsDataAsset;

	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input Functions
	void Move2D(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	
	void UsePrimaryAttack(const FInputActionValue& Value);
	void UseSpecialAbility(const FInputActionValue& Value);
	void UseMovementAbility(const FInputActionValue& Value);
	
	void StartSprint();
	void StopSprint();

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);
public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;
	

};
