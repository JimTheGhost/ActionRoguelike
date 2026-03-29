// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameplayFunctionLibrary.h"
#include "ARLAttributeComponent.h"

bool UARLGameplayFunctionLibrary::ApplyDamage(AActor* DamageDealer, AActor* TargetActor, float DamageAmount)
{
	UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(-DamageAmount, DamageDealer);
	}
	return false;
}

bool UARLGameplayFunctionLibrary::ApplyDirectionDamage(AActor* DamageDealer, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageDealer, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			UE_LOG(LogTemp, Log, TEXT("Component Hit and Simulating: %s"), *HitComponent->GetName());
			HitComponent->AddImpulseAtLocation(Direction * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
