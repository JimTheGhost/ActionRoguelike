// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARLBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Check distance between AI Pawn and Target Actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")))
		{
			AAIController* OwnerController = OwnerComp.GetAIOwner();
			if (ensure(OwnerController))
			{
				APawn* OwnerPawn = OwnerController->GetPawn();
				if (ensure(OwnerPawn))
				{
					float DistanceToTarget = FVector::Distance(TargetActor->GetActorLocation(), OwnerPawn->GetActorLocation());

					bool bWithinRange = DistanceToTarget <= AttackRange;
					bool bHasLOS = false;
					
					if (bWithinRange)
					{
						bHasLOS = OwnerController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(WithinRangeBool.SelectedKeyName, bWithinRange && bHasLOS);
				}
			}
		}
	}
	//Return boolean true if within defined range
}
