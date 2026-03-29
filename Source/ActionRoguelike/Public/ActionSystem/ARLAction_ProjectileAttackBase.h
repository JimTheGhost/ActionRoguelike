// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ARLAction.h"
#include "ARLAction_ProjectileAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLAction_ProjectileAttackBase : public UARLAction
{
	GENERATED_BODY()
public:
	UARLAction_ProjectileAttackBase();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName SpawnSocket;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* CastEmitter;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AbilityAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTiming = 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
