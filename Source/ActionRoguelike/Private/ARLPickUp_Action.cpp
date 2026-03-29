// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPickUp_Action.h"

#include "ActionSystem/ARLActionComponent.h"


// Sets default values
AARLPickUp_Action::AARLPickUp_Action()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AARLPickUp_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	UARLActionComponent* ActionComp = UARLActionComponent::GetActionComponent(InstigatorPawn);
	if (ActionComp)
	{
		if (!ActionComp->DoesActionExist(ActionClass))
		{
			ActionComp->AddAction(InstigatorPawn, ActionClass);
			Super::Interact_Implementation(InstigatorPawn);
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Action Exists!"));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Action Component found!"));
}

