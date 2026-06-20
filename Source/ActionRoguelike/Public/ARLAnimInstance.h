// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARLAnimInstance.generated.h"

/**
 * 
 */
class UARLActionComponent;
UCLASS()
class ACTIONROGUELIKE_API UARLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	 
	protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	bool IsStunned;
	
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	UARLActionComponent* ActionComp;
	
	void NativeInitializeAnimation() override;
	
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
