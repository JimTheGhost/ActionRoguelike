// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARLBTService_CheckIfInjured.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLBTService_CheckIfInjured : public UBTService
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float HealthThreshold;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InjuredBoolKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
