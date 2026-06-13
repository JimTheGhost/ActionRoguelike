// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void TogglePauseMenu();
	
	void SetupInputComponent() override;
public:
	virtual void BeginPlayingState() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void BlueprintBeginPlayingState();
};
