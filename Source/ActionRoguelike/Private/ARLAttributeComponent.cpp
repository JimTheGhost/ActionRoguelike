// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

#include "ARLGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

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
	
	SetIsReplicatedByDefault(true);
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
	}
	
	float OldHealth = CurrentHealth;
	float NewHealth = FMath::Clamp(CurrentHealth + Delta, 0, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;
	
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = NewHealth;
		
		//broadcast health change to delegates
		if (ActualDelta != 0.0f){
			MulticastHealthChanged(InstigatorActor, CurrentHealth, ActualDelta);
		}
		
		if (ActualDelta < 0.0f)
		{
			ApplyRageChange(10, InstigatorActor);
			//Check if dead and report to game mode
			if (CurrentHealth <= 0.0f)
			{
				AARLGameModeBase* GM = GetWorld()->GetAuthGameMode<AARLGameModeBase>();
				if (GM)
				{
					GM->OnActorKilled(GetOwner(), InstigatorActor);
				}
			}
		}
	}
	return ActualDelta != 0.0f;
}

bool UARLAttributeComponent::ApplyRageChange(float Delta, AActor* InstigatorActor)
{
	float OldRage = CurrentRage;
	float NewRage = FMath::Clamp(CurrentRage + Delta, 0, MaxRage);
	float ActualDelta = NewRage - OldRage;
	if (GetOwner()->HasAuthority())
	{
		CurrentRage = NewRage;
		if (ActualDelta != 0.0f)
		{
			MulticastRageChanged(InstigatorActor, CurrentRage, ActualDelta);
		}
	}
	return ActualDelta != 0.0f;
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

void UARLAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void UARLAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
                                                                   float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

UARLAttributeComponent* UARLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor == nullptr)
	{
		return nullptr;
	}
	return FromActor->FindComponentByClass<UARLAttributeComponent>();
}

void UARLAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UARLAttributeComponent, CurrentHealth);
	DOREPLIFETIME(UARLAttributeComponent, MaxHealth);
	DOREPLIFETIME(UARLAttributeComponent, CurrentRage);
	DOREPLIFETIME(UARLAttributeComponent, MaxRage);
}
