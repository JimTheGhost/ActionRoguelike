// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLExplosiveBarrel.h"
#include "ARLMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARLExplosiveBarrel::AARLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComp->Radius = 1000;
	RadialForceComp->ImpulseStrength = 5000;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->SetupAttachment(RootComponent);

}

void AARLExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this,&AARLExplosiveBarrel::HitEventDelegate);
}

void AARLExplosiveBarrel::HitEventDelegate(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AARLMagicProjectile>(OtherActor))
	{
		ExplodeBarrel();
	}
	//UE_LOG(LogTemp, Log, TEXT("Hit Detected"));

	//%s = string
	//%f = float
	
	//UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	//FString CombinedString = FString::Printf(TEXT("Hit target at location: %s"), *Hit.ImpactPoint.ToString());
	//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

void AARLExplosiveBarrel::ExplodeBarrel()
{
	TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(),-50.0f,GetActorLocation() + FVector(0,0,50),RadialForceComp->Radius, UDamageType::StaticClass(),IgnoredActors,0,0,true);
	RadialForceComp->FireImpulse();


}

