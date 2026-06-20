// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLAction.h"

#include "ARLAttributeComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionSystem/ARLActionComponent.h"
#include "Net/UnrealNetwork.h"

void UARLAction::Initialize(UARLActionComponent* NewActionComponent)
{
	ActionComp = NewActionComponent;
}

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Starting: %s"), *ActionName.ToString()), FColor::Green);
	
	UARLActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantTags);
	
	if (bUseCost)
	{
		UARLAttributeComponent::GetAttributes(OwningComp->GetOwner())->ApplyRageChange(-ResourceCost, Instigator);
	}

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
	
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(),this);
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Stopping: %s"), *ActionName.ToString()), FColor::White);
	
	UARLActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(),this);
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return ActionComp;
	//return Cast<UARLActionComponent>(GetOuter());
}

void UARLAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}else
	{
		StopAction(RepData.Instigator);
	}
}


bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning)
	{
		return false;
	}
	
	UARLActionComponent* OwningComp = GetOwningComponent();
	if (OwningComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	if (bUseCost)
	{
		if (UARLAttributeComponent::GetAttributes(OwningComp->GetOwner())->GetCurrentRage() < ResourceCost)
		{
			return false;
		}
	}
	return true;
}

bool UARLAction::IsRunning() const
{
	return RepData.bIsRunning;
}

UWorld* UARLAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

void UARLAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UARLAction, RepData);
	DOREPLIFETIME(UARLAction, ActionComp);
	DOREPLIFETIME(UARLAction, TimeStarted);
}
