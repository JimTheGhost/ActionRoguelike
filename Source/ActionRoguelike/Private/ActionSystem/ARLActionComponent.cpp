// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLActionComponent.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionSystem/ARLAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UARLActionComponent::UARLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
}

void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		for ( TSubclassOf<UARLAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

bool UARLActionComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARLAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UARLActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass))
		return;
	}
	
	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UARLActionComponent::RemoveAction(UARLAction* ActionClass)
{	
	if (ensure(ActionClass && !ActionClass->IsRunning()))
	{
		Actions.Remove(ActionClass);
	}

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
			
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
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
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
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

void UARLActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UARLActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

// Called every frame
void UARLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugString = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, DebugString);
	
	for (UARLAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning()? FColor::Blue : FColor::White;
		
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), 
			*GetNameSafe(GetOwner()), 
			*GetNameSafe(Action));
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UARLActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UARLActionComponent, Actions);
}

