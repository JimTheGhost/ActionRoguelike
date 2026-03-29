// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ARLAction_ProjectileAttackBase.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UARLAction_ProjectileAttackBase::UARLAction_ProjectileAttackBase()
{
	SpawnSocket = "Muzzle_01";
	AttackTiming = 0.2f;
}

void UARLAction_ProjectileAttackBase::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = InstigatorCharacter;
		SpawnParams.Instigator = InstigatorCharacter;
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		
		FCollisionObjectQueryParams TraceParams;
		TraceParams.AddObjectTypesToQuery(ECC_Pawn);
		TraceParams.AddObjectTypesToQuery(ECC_WorldStatic);
		TraceParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	
		FVector LineStart = InstigatorCharacter->GetPawnViewLocation();
		FVector LineEnd = LineStart + (InstigatorCharacter->GetControlRotation().Vector() * 3000);
		
		FHitResult HitResult;
		if (GetWorld()->SweepSingleByObjectType(HitResult, LineStart, LineEnd, FQuat::Identity, TraceParams, Shape, Params))
		{
			LineEnd = HitResult.ImpactPoint;
		}
	
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(SpawnSocket);
		FRotator FireRotation = FRotationMatrix::MakeFromX(LineEnd - HandLocation).Rotator();
		
		FTransform SpawnTM = FTransform(FireRotation,HandLocation,FVector(1,1,1));
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}

void UARLAction_ProjectileAttackBase::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (ACharacter* Character = Cast<ACharacter>(Instigator))
	{
		Character->PlayAnimMontage(AbilityAnim);

		UGameplayStatics::SpawnEmitterAttached(CastEmitter, Character->GetMesh(),SpawnSocket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle AttackDelay_Handle;
		FTimerDelegate Delegate;
		
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(AttackDelay_Handle, Delegate, AttackTiming, false);
	}

}
