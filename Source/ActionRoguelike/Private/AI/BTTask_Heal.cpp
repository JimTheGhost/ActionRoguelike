// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Heal.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if (ensure(MyController))
	{
		APawn* MyPawn = MyController->GetPawn();
		
		if (UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(MyPawn))
		{
			AttributeComponent->ApplyHealthChange(HealAmount, MyPawn);

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
