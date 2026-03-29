// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AARLDashProjectile::AARLDashProjectile()
{
	//Initial Constructor
	TeleportLocation = FVector(0, 0, 0);
	DelayTime = 0.2f;
	KillTime = 0.2f;
}

void AARLDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorldTimerManager().SetTimer(KillTimer, this, &AARLDashProjectile::PrepareTeleport, KillTime);
}

void AARLDashProjectile::ProjectileHitDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::ProjectileHitDelegate(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	PrepareTeleport();
	
}

void AARLDashProjectile::PrepareTeleport()
{
	ParticleComp->Deactivate();
	MovementComp->Deactivate();
	GetWorldTimerManager().ClearTimer(KillTimer);
	UE_LOG(LogTemp, Log, TEXT("Preparing Teleport"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEmitter,GetActorLocation(), GetActorRotation(), FVector(1), true);
	TeleportLocation = GetActorLocation();
	GetWorldTimerManager().SetTimer(DelayTimer, this, &AARLDashProjectile::TeleportAction, DelayTime);
}

void AARLDashProjectile::TeleportAction()
{
	if (GetInstigator()->TeleportTo(TeleportLocation, GetActorRotation()))
	{
		UE_LOG(LogTemp, Log, TEXT("Successful Teleport"));
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed Teleport"));
	}
	K2_DestroyActor();
}