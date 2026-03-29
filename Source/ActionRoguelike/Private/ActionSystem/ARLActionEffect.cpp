// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLActionEffect.h"
#include "ActionSystem/ARLActionComponent.h"

UARLActionEffect::UARLActionEffect()
{
	bAutoStart = true;
}

void UARLActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, FName("StopAction"), Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, FName("ExecutePeriodicEffect"), Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UARLActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
		
	Super::StopAction_Implementation(Instigator);
	
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (UARLActionComponent* ActionComp = GetOwningComponent())
	{
		ActionComp->RemoveAction(this);
	}
}

void UARLActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}
