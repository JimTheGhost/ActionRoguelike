// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLActionEffect : public UARLAction
{
	GENERATED_BODY()
public:
	UARLActionEffect();
	
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	float Period;
	
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effects")
	void ExecutePeriodicEffect(AActor* Instigator);
};
