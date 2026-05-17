// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AARLProjectileBase::AARLProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("Collision");
	SphereComp->SetCollisionProfileName(FName("Projectile"));
	RootComponent = SphereComp;
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("FX");
	ParticleComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);
	AudioComp->bAutoActivate = false;

	ImpactInnerRadius = 250.0f;
	ImpactOuterRadius = 4000.0f;
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AARLProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	AudioComp->Play();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AARLProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AARLProjectileBase::ProjectileOverlapDelegate);
	SphereComp->OnComponentHit.AddDynamic(this,&AARLProjectileBase::ProjectileHitDelegate);
}

void AARLProjectileBase::ProjectileOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AARLProjectileBase::ProjectileHitDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AARLProjectileBase::Explode_Implementation()
{
	UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), ImpactInnerRadius, ImpactOuterRadius);
	UGameplayStatics::SpawnEmitterAtLocation(this, HitEmitter,GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	Destroy();
}
