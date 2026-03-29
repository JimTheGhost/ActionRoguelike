// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAbilityData.generated.h"

USTRUCT(BlueprintType)
struct FARLAbilityData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnSocket;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AbilityAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTiming = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* CastEmitter;
};