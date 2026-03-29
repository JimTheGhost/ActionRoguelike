// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLActionEffect.h"
#include "ARLActionEffect_Thorns.generated.h"

class AARLAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLActionEffect_Thorns : public UARLActionEffect
{
	GENERATED_BODY()
	
	UARLActionEffect_Thorns();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	UFUNCTION()
	void OnHealthChanged(AActor* Instigator, UARLAttributeComponent* OwningComp, float NewHealth, float Delta);
	
protected:
	float ThornsDamagePercent;
};
