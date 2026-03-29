// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLActionEffect_Thorns.h"

#include "ARLAttributeComponent.h"
#include "ARLGameplayFunctionLibrary.h"

UARLActionEffect_Thorns::UARLActionEffect_Thorns()
{
	ThornsDamagePercent = 0.25f;
}

void UARLActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(Instigator);
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UARLActionEffect_Thorns::OnHealthChanged);
	}
}

void UARLActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(Instigator);
	if (AttributeComponent)
	{
	AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UARLActionEffect_Thorns::OnHealthChanged);
	}
}

void UARLActionEffect_Thorns::OnHealthChanged(AActor* Instigator, UARLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = OwningComp->GetOwner();
	if (ensureAlways(OwningActor) && Instigator)
	{
		if (Delta < 0.0f && Instigator != OwningActor)
		{
			UARLGameplayFunctionLibrary::ApplyDamage(OwningActor, Instigator, FMath::RoundToInt(FMath::Abs(Delta) * ThornsDamagePercent));
		}
	}
}
