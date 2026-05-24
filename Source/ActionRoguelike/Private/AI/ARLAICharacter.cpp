// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "ARLWorldUserWidget.h"
#include "BrainComponent.h"
#include "ActionSystem/ARLActionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize Components
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UARLActionComponent>("ActionComp");


	//Initialize AI Possession
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParam = FName("HitTime");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
}

void AARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Bind functions to delegates
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARLAICharacter::OnHealthChange);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AARLAICharacter::OnPawnSeen);
}

void AARLAICharacter::SetTargetActor(AActor* TargetActor)
{
	if (AAIController* MyController = Cast<AAIController>(GetController()))
	{
		UObject* CurrentTarget = MyController->GetBlackboardComponent()->GetValueAsObject("TargetActor");
		if (CurrentTarget != TargetActor)
		{
			MyController->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
			CreateDetectionWidget();
		}
	}
}

void AARLAICharacter::OnHealthChange(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth,
                                     float Delta)
{
	//only do these actions on negative health change
	if (Delta < 0.0f)
	{
		//target the player that last damaged this
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		//Spawn health bar if one does not exist
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UARLWorldUserWidget>(GetWorld(), HealthBarClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParam, GetWorld()->GetTimeSeconds());
		
		if (NewHealth <= 0.0f)
		{
			//Stop behavior tree
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Zero Health");
			}
			//Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			if (ActiveHealthBar)
			{
				ActiveHealthBar->RemoveFromParent();
			}

			//Set lifespan for cleanup
			SetLifeSpan(10.0f);
		}
	}
}

void AARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void AARLAICharacter::CreateDetectionWidget_Implementation()
{
	if (ActiveDetectionWidget == nullptr)
	{
		ActiveDetectionWidget = CreateWidget<UARLWorldUserWidget>(GetWorld(), DetectionWidgetClass);
		if (ActiveDetectionWidget)
		{
			ActiveDetectionWidget->AttachedActor = this;
			ActiveDetectionWidget->AddToViewport();
		}
	}
}



