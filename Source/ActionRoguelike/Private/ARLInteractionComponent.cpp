// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"

#include "ARLGameplayInterface.h"
#include "EngineUtils.h"
#include "ARLWorldUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

static TAutoConsoleVariable<bool> CVarEnableInteractDebug(TEXT("arl.InteractDebug"), true, TEXT("Enable or Disable debug shapes for interaction"));

// Sets default values for this component's properties
UARLInteractionComponent::UARLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InteractRange = 500.0f;
	TraceRadius = 50.0f;
	TraceChannel = ECC_WorldDynamic;

	// ...
}
void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UARLInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarEnableInteractDebug.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceChannel);
	
	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * InteractRange);
	
	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	FocusedActor = nullptr;
	
	for (FHitResult& Hit : Hits)
	{
		if (bDrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false);
		}
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UARLGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	if (FocusedActor)
	{
		if (InteractWidget == nullptr && ensure(InteractWidgetClass))
		{
			InteractWidget = CreateWidget<UARLWorldUserWidget>(GetWorld(), InteractWidgetClass);
		}
		
		if (InteractWidget)
		{
			InteractWidget->AttachedActor = FocusedActor;
			
			if (!InteractWidget->IsInViewport())
			{
				InteractWidget->AddToViewport();
			}
			
		}
	}else
	{
		if (InteractWidget)
		{
			InteractWidget->RemoveFromParent();
		}
	}
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, -1.0f,0.0f, 2.0f);
	}
}

void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UARLInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UARLInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
		if (InFocus == nullptr)
    	{
    		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Interact: No Actor Found"));
    		return;
    	}
    	
    	if (APawn* MyPawn = Cast<APawn>(GetOwner()))
    	{
    		IARLGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
    	}
}
