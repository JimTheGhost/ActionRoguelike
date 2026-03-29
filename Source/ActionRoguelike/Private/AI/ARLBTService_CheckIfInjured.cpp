// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTService_CheckIfInjured.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARLBTService_CheckIfInjured::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* OwnerController = OwnerComp.GetAIOwner();
		if (ensure(OwnerController))
		{
			APawn* OwnerPawn = OwnerController->GetPawn();
			if (ensure(OwnerPawn))
			{
				if (UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(OwnerPawn))
				{
					bool bIsInjured = AttributeComponent->GetCurrentHealth()<=HealthThreshold;
					
					BlackboardComp->SetValueAsBool(InjuredBoolKey.SelectedKeyName, bIsInjured);
				}
			}
		}
	}
}
