// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLActionComponent.h"

#include "ActionSystem/ARLAction.h"

UARLActionComponent::UARLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARLActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	
	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
		
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UARLActionComponent::RemoveAction(UARLAction* ActionClass)
{	
	if (!ensure(ActionClass && !ActionClass->IsRunning()))
	Actions.Remove(ActionClass);
}

UARLActionComponent* UARLActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor == nullptr)
	{
		return nullptr;
	}
	return FromActor->FindComponentByClass<UARLActionComponent>();
}

bool UARLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString DebugString = FString::Printf(TEXT("Failed to start: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugString);
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UARLActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UARLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

bool UARLActionComponent::DoesActionExist(TSubclassOf<UARLAction> ActionClass)
{
	for (UARLAction* Action : Actions)
	{
		if (Action && Action->GetClass() == ActionClass)
		{
			return true;
		}
	}
	return false;
}

void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for ( TSubclassOf<UARLAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


// Called every frame
void UARLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugString = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, DebugString);
}

