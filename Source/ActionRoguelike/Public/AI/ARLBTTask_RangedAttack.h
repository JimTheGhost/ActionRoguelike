// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLProjectileBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARLBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpreadDeg;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AARLProjectileBase> ProjectileClass;

public:
	UARLBTTask_RangedAttack();
};
