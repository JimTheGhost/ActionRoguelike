// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTTask_RangedAttack.h"

#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UARLBTTask_RangedAttack::UARLBTTask_RangedAttack()
{
	MaxBulletSpreadDeg = 8.0f;
}

EBTNodeResult::Type UARLBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
		if (MyCharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!UARLAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		
		FVector DirectionToTarget = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator FireRotation = DirectionToTarget.Rotation();
		FireRotation.Pitch += FMath::RandRange(-MaxBulletSpreadDeg, MaxBulletSpreadDeg);
		FireRotation.Yaw += FMath::RandRange(-MaxBulletSpreadDeg, MaxBulletSpreadDeg);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = MyCharacter;
		SpawnParams.Instigator = MyCharacter;

		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, FireRotation, SpawnParams);

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


