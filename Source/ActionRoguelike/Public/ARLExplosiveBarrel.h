// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARLExplosiveBarrel();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForceComp;

	UFUNCTION(BlueprintCallable)
	void ExplodeBarrel ();
protected:

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void HitEventDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
