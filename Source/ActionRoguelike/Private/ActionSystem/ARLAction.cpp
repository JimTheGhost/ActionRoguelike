// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLAction.h"

#include "ARLAttributeComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionSystem/ARLActionComponent.h"
#include "Net/UnrealNetwork.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Starting: %s"), *ActionName.ToString()), FColor::Green);
	
	UARLActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantTags);
	
	if (bUseCost)
	{
		UARLAttributeComponent::GetAttributes(OwningComp->GetOwner())->ApplyRageChange(-ResourceCost, Instigator);
	}

	bIsRunning = true;
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopping: %s"), *ActionName.ToString()), FColor::White);
	
	UARLActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return Cast<UARLActionComponent>(GetOuter());
}

void UARLAction::OnRep_IsRunning()
{
//@FIXME: pass in proper instigator
	if (bIsRunning)
	{
		StartAction(nullptr);
	}else
	{
		StopAction(nullptr);
	}
}

bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	if (bIsRunning)
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
	return bIsRunning;
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
	
	DOREPLIFETIME(UARLAction, bIsRunning);
}
