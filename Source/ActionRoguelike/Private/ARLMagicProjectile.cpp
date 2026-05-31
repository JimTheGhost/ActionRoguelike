// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMagicProjectile.h"

#include "ARLAttributeComponent.h"
#include "ARLGameplayFunctionLibrary.h"
#include "ActionSystem/ARLActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ActionSystem/ARLActionEffect.h"

// Sets default values
AARLMagicProjectile::AARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
void AARLMagicProjectile::ProjectileOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::ProjectileOverlapDelegate(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if(OtherActor && OtherActor != GetInstigator())
	{
		UARLActionComponent* ActionComponent = Cast<UARLActionComponent>(OtherActor->GetComponentByClass(UARLActionComponent::StaticClass()));
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (UARLGameplayFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();
			
			if (ActionComponent && HasAuthority())
			{
				ActionComponent->AddAction(GetInstigator(), OnHitEffect);
			}
		}
	}
}

void AARLMagicProjectile::ProjectileHitDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::ProjectileHitDelegate(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	if(OtherActor && OtherActor != GetInstigator())
	{
		UARLGameplayFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor, DamageAmount, Hit);
		Explode();
	}


}

