// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UAudioComponent;


UCLASS()
class ACTIONROGUELIKE_API AARLProjectileBase : public AActor
{
	GENERATED_BODY()
protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, Category = "FX|Impact")
	USoundBase* ImpactSound;
	
	UPROPERTY(EditAnywhere, Category = "FX|Impact|Camera")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;
	UPROPERTY(EditAnywhere, Category = "FX|Impact|Camera")
	float ImpactOuterRadius;
	UPROPERTY(EditAnywhere, Category = "FX|Impact|Camera")
	float ImpactInnerRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX|Impact")
	UParticleSystem* HitEmitter;
public:
	
	// Sets default values for this actor's properties
	AARLProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void ProjectileOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ProjectileHitDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();


};
