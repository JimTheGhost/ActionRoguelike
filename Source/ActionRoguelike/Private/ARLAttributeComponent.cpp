// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

#include "ARLGameModeBase.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<float> CVarGlobalDamageMultiplier(TEXT("arl.GDamageMult"), 1.0f, TEXT("Multiply all damage done by input"));

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = 100;
	MaxHealth = 100;
	CurrentRage = 0;
	MaxRage = 100;
}

//Action Functions
bool UARLAttributeComponent::ApplyHealthChange(float Delta, AActor* InstigatorActor)
{
	//Check if in damagable
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarGlobalDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
		ApplyRageChange(10, InstigatorActor);
	}
	
	float OldHealth = CurrentHealth;
	
	//UE_LOG(LogTemp, Log, TEXT("Applying health change %f"), Delta);
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0, MaxHealth);
	//UE_LOG(LogTemp, Log, TEXT("New Health %f"), CurrentHealth);

	//broadcast health change to delegates
	float ActualDelta = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, ActualDelta);

	//Check if dead and report to game mode
	if (ActualDelta < 0.0f && CurrentHealth <= 0.0f)
	{
		AARLGameModeBase* GM = GetWorld()->GetAuthGameMode<AARLGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	return true;
}

bool UARLAttributeComponent::ApplyRageChange(float Delta, AActor* InstigatorActor)
{
	float OldRage = CurrentRage;
	CurrentRage = FMath::Clamp(CurrentRage + Delta, 0, MaxRage);
	OnRageChanged.Broadcast(InstigatorActor, this, CurrentRage, Delta);
	float ActualDelta = CurrentRage - OldRage;
	return true;
}

bool UARLAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(-GetMaxHealth(), Instigator);
}

//Checker Functions
bool UARLAttributeComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

bool UARLAttributeComponent::IsFullHealth() const
{
	return CurrentHealth == MaxHealth;
}

bool UARLAttributeComponent::IsActorAlive(AActor* TargetActor)
{
	if (UARLAttributeComponent* AC = GetAttributes(TargetActor))
	{
		return AC->IsAlive();
	}
	return false;
}

//Getter Functions
float UARLAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UARLAttributeComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UARLAttributeComponent::GetMaxRage() const
{
	return MaxRage;
}

float UARLAttributeComponent::GetCurrentRage() const
{
	return CurrentRage;
}

UARLAttributeComponent* UARLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor == nullptr)
	{
		return nullptr;
	}
	return FromActor->FindComponentByClass<UARLAttributeComponent>();
}
