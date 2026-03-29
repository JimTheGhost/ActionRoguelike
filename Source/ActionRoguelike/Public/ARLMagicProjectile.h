// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLProjectileBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ARLMagicProjectile.generated.h"

class UARLActionEffect;

UCLASS()
class ACTIONROGUELIKE_API AARLMagicProjectile : public AARLProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARLMagicProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float DamageAmount;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
private:


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UARLActionEffect> OnHitEffect;
	
	virtual void ProjectileOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult) override;
	
	virtual void ProjectileHitDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
