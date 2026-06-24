// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ARLMonsterData.generated.h"

class UARLAction;
class AARLAICharacter;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Attributes")
	TSubclassOf<AARLAICharacter> MonsterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Attributes")
	TArray<TSubclassOf<UARLAction>> Actions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UTexture2D* Icon;
};
