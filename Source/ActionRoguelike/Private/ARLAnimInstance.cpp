// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAnimInstance.h"

#include "GameplayTagContainer.h"
#include "ActionSystem/ARLActionComponent.h"

void UARLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UARLActionComponent>(OwningActor->GetComponentByClass(UARLActionComponent::StaticClass()));
	}
}

void UARLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	
	if (ActionComp)
	{
		IsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
