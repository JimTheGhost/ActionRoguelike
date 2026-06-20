// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ARLAction.generated.h"

class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsRunning = false;
	UPROPERTY()
	AActor* Instigator = nullptr;
};
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARLAction : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	
	UPROPERTY(Replicated)
	UARLActionComponent* ActionComp;
	
	UPROPERTY(Replicated)
	float TimeStarted;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UARLActionComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantTags;
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	UPROPERTY(EditDefaultsOnly, Category = "Cost")
	bool bUseCost;
	UPROPERTY(EditDefaultsOnly, Category = "Cost")
	float ResourceCost;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	
	UFUNCTION()
	void OnRep_RepData();

	public:
	void Initialize(UARLActionComponent* NewActionComponent);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
	bool bAutoStart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void StartAction(AActor* Instigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;
	
	UWorld* GetWorld() const override;
	
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
