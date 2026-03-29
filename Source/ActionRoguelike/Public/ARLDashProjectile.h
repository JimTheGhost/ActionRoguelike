// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLProjectileBase.h"
#include "ARLDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLDashProjectile : public AARLProjectileBase
{
	GENERATED_BODY()
public:
	AARLDashProjectile();
	
protected:
	FVector TeleportLocation;

	UPROPERTY(EditAnywhere, Category ="Attributes")
	float DelayTime;

	UPROPERTY(EditAnywhere, Category ="Attributes")
	float KillTime;

	FTimerHandle DelayTimer;
	FTimerHandle KillTimer;
	
	virtual void BeginPlay() override;
	
	virtual void ProjectileHitDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	void PrepareTeleport();
	void TeleportAction();
};
