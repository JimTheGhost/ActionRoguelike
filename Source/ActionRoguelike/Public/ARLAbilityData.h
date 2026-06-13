// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAbilityData.generated.h"

USTRUCT(BlueprintType)
struct FARLAbilityData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityName = "None";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnSocket = "None";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AbilityAnim = nullptr;
;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTiming = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* CastEmitter = nullptr;
;
};