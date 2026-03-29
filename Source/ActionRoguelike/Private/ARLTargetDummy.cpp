// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTargetDummy.h"
#include "ARLAttributeComponent.h"

// Sets default values
AARLTargetDummy::AARLTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Initialize Components
	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComp");
	RootComponent = BaseMeshComp;
	
	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");

}

void AARLTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARLTargetDummy::OnHealthChange);
}

void AARLTargetDummy::OnHealthChange(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	BaseMeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}


