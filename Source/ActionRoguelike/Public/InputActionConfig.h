// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputActionConfig.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UInputActionConfig : public UDataAsset
{
	GENERATED_BODY()
	public:	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* PrimaryAttackAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* SpecialAbilityAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* MovementAbilityAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* SprintAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* ParryAction;


};
