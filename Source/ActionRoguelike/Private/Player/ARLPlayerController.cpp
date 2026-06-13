// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ARLPlayerController.h"
#include "Blueprint/UserWidget.h"

void AARLPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		UE_LOG(LogTemp, Warning,TEXT("Menu Destroyed"));
		SetPause(false);
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this,PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
			
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		SetPause(true);
	}
}

void AARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AARLPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	BlueprintBeginPlayingState();
}

void AARLPlayerController::BlueprintBeginPlayingState_Implementation()
{
}
