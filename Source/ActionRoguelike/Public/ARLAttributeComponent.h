// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UARLAttributeComponent*, OwningComp, float, NewValue, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UARLAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARLAttributeComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta, AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(float Delta, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UARLAttributeComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	float GetCurrentHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes|Rage")
	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes|Rage")
	float GetCurrentRage() const;
	
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes|Status")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes|Status")
	float CurrentRage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxRage;
	
	UFUNCTION(NetMulticast, Reliable) //@FIXME: change to `Unreliable` once death state is moved out of character class
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);
};
