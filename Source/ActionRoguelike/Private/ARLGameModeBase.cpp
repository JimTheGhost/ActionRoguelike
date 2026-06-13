// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"

#include "ARLAttributeComponent.h"
#include "ARLCharacter.h"
#include "ARLGameplayInterface.h"
#include "ARLSaveGame.h"
#include "EngineUtils.h"
#include "AI/ARLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ARLPlayerState.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("arl.SpawnBots"), true, TEXT("Set bot spawner to active (1) or inactive (0)"), ECVF_Cheat);

AARLGameModeBase::AARLGameModeBase()
{
	SpawnInterval = 2.0f;
	CreditsPerKill = 10;
	
	SlotName = "SaveGame01";
}

void AARLGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	LoadSaveGame();
}

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotTimer, this, &AARLGameModeBase::SpawnBotTimerElapsed, SpawnInterval, true);
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this,EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARLGameModeBase::OnPowerUpQueryCompleted);
	}
}

void AARLGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
	AARLPlayerState* PS = NewPlayer->GetPlayerState<AARLPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

void AARLGameModeBase::KillAllBots()
{
	for (TActorIterator<AARLAICharacter> It(GetWorld()); It; ++It)
	{
		AARLAICharacter* Bot = *It;

		UARLAttributeComponent* AttributeComponent = UARLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); // @todo: pass in player for kill credit
		}
	}
}

void AARLGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawner Disabled via CVar 'arl.SpawnBots'"));
		return;
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this,EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARLGameModeBase::OnQueryCompleted);
	}
}




void AARLGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                        EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS has failed!"));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<AARLAICharacter> It(GetWorld()); It; ++It)
	{
		if (AARLAICharacter* Bot = *It; UARLAttributeComponent::IsActorAlive(Bot))
		{
			NrOfAliveBots++;
		}
	}

	float MaxNrOfBots = 10.0f;
	if (DifficultyCurve)
	{
		MaxNrOfBots = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (NrOfAliveBots >= MaxNrOfBots)
	{
		return;
	}
	
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	
	if (SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(BotClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}

void AARLGameModeBase::OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) return;
	
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	
	if (SpawnLocations.Num() == 0) return;
	bool bSpawnPowerUps = true;
	
	while (bSpawnPowerUps)
	{
		if (SpawnLocations.Num() == 0 || UsedLocations.Num() >= MaxPowerUps)
		{
			bSpawnPowerUps = false;
			continue;
		}
		int32 RandomIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		
		FVector PickedLocation = SpawnLocations[RandomIndex];
		SpawnLocations.RemoveAt(RandomIndex);
		
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float Distance = (PickedLocation-OtherLocation).Size();
			if (Distance < PowerUpSpacing)
			{
				bValidLocation = false;
				break;
			}
		}
		if (!bValidLocation)
		{
			continue;
		}
		
		int32 RandomItemIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1);
		AActor* PowerUp = GetWorld()->SpawnActor<AActor>(PowerUpClasses[RandomItemIndex], PickedLocation, FRotator::ZeroRotator);
		if (PowerUp)
		{
			UsedLocations.Add(PickedLocation);
		}
		
	}
}

void AARLGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AARLGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	
	if (AARLCharacter* Player = Cast<AARLCharacter>(Victim))
	{
		FTimerHandle RespawnTimerHandle;
		
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("RespawnPlayerElapsed"), Player->GetController(), RespawnTimerHandle);

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, TimerDelegate, RespawnDelay, false);
	}
	UE_LOG(LogTemp, Log, TEXT("%s Killed by %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
	if (APawn* KillerPawn = Cast<APawn>(Killer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Pawn %s"), *GetNameSafe(Killer));
		if (AARLPlayerState* PlayerState = Cast<AARLPlayerState>(KillerPawn->GetPlayerState()))
		{
			UE_LOG(LogTemp, Log, TEXT("Found PlayerState %s"), *GetNameSafe(PlayerState));
			PlayerState->UpdateCredits(CreditsPerKill);
		}
	}
}

void AARLGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AARLPlayerState* PlayerState = Cast<AARLPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
		}
	}
	
	CurrentSaveGame->SavedActors.Empty();
	
	//iterate over entire worlds actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		//only concerned with 'Gameplay actors'
		if (!Actor->Implements<UARLGameplayInterface>())
		{
			continue;
		}
		
		UE_LOG(LogTemp, Log, TEXT("Attempting Save on %s"), *GetNameSafe(Actor))
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetActorTransform();
		
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemWriter,true);
		Archive.ArIsSaveGame = true;
		
		Actor->Serialize(Archive);
		
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AARLGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARLSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."));
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame data."))
		
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			//only concerned with 'Gameplay actors'
			if (!Actor->Implements<UARLGameplayInterface>())
			{
				continue;
			}
			
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.ActorTransform);
					
					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Archive(MemReader,true);
					Archive.ArIsSaveGame = true;
		
					Actor->Serialize(Archive);
					
					IARLGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	} 
	else
	{
		CurrentSaveGame = Cast<UARLSaveGame>(UGameplayStatics::CreateSaveGameObject(UARLSaveGame::StaticClass()));
		
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data."))
	}
}

