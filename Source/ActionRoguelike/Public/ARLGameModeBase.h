// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "ARLGameModeBase.generated.h"

class AARLAICharacter;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AARLGameModeBase();
	
protected:
	FTimerHandle SpawnBotTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnInterval;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 CreditsPerKill;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AARLAICharacter> BotClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	TArray<TSubclassOf<AActor>> PowerUpClasses;
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	UEnvQuery* PowerUpQuery;
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	int32 MaxPowerUps;
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	float PowerUpSpacing;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void SpawnBotTimerElapsed();
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
public:
	virtual void OnActorKilled(AActor* Victim, AActor* Killer);
	
	virtual void StartPlay() override;

	UFUNCTION(Exec, Category = "Cheats")
	void KillAllBots();
};
