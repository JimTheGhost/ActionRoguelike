// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AARLAIController::AARLAIController()
{
}

void AARLAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign a Behavior tree to your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}

}


