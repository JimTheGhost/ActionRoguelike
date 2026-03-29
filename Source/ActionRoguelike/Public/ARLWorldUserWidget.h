// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARLWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector Offset;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;
	protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
