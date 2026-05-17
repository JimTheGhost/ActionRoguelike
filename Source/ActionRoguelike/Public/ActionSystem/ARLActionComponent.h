// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ARLActionComponent.generated.h"

class UARLAction;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	// Sets default values for this component's properties
	UARLActionComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UARLAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UARLAction* ActionClass);
	UFUNCTION(BlueprintCallable, Category = "Actions")
	static UARLActionComponent* GetActionComponent(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool DoesActionExist(TSubclassOf<UARLAction> ActionClass);


protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UARLAction>> DefaultActions;
	
	UPROPERTY()
	TArray<UARLAction*> Actions;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
