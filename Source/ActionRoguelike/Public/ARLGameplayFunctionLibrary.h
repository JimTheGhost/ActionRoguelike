// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARLGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* DamageDealer, AActor* TargetActor, float DamageAmount);
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionDamage(AActor* DamageDealer, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);
};
