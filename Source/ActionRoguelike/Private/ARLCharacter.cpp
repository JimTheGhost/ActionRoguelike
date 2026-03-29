// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "ARLAttributeComponent.h"
#include "ARLInteractionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionConfig.h"
#include "ActionSystem/ARLActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AARLCharacter::AARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComp->SetupAttachment(SpringArmComp);

	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComponent");
	ActionComp = CreateDefaultSubobject<UARLActionComponent>("ActionComponent");

	InteractionComponent = CreateDefaultSubobject<UARLInteractionComponent>("InteractionComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AARLCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(Amount, this);
}

void AARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARLCharacter::OnHealthChange);
}

FVector AARLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void AARLCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext,0);
		}
	}
}

void AARLCharacter::Move2D(const FInputActionValue& Value)
{
	const FRotator YawControlRotator = FRotator(0,GetControlRotation().Yaw,0);
	const FVector MoveDir = YawControlRotator.RotateVector(FVector(Value.Get<FVector2D>(),0));
	AddMovementInput(MoveDir,1,false);
}

void AARLCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D MouseInput = Value.Get<FVector2D>();
	AddControllerYawInput(MouseInput.X);
	AddControllerPitchInput(MouseInput.Y);
}

void AARLCharacter::Interact(const FInputActionValue& Value)
{
	InteractionComponent->PrimaryInteract();
}

void AARLCharacter::UsePrimaryAttack(const FInputActionValue& Value)
{
	ActionComp->StartActionByName(this,"PrimaryAttack");
}

void AARLCharacter::UseSpecialAbility(const FInputActionValue& Value)
{
	ActionComp->StartActionByName(this,"SpecialAbility");
}

void AARLCharacter::UseMovementAbility(const FInputActionValue& Value)
{
	ActionComp->StartActionByName(this,"MovementAbility");
}

void AARLCharacter::StartSprint()
{
	ActionComp->StartActionByName(this, FName("Sprint"));
}

void AARLCharacter::StopSprint()
{
	ActionComp->StopActionByName(this, FName("Sprint"));
}

void AARLCharacter::OnHealthChange(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth,
                                   float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("HitTime", GetWorld()->TimeSeconds);
		GetMesh()->SetScalarParameterValueOnMaterials("HitScale", 5.0f);
		
		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}

}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent
		= CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputActionsDataAsset->MoveAction,ETriggerEvent::Triggered,this, &AARLCharacter::Move2D);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->LookAction,ETriggerEvent::Triggered,this, &AARLCharacter::Look);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->JumpAction,ETriggerEvent::Triggered,this, &AARLCharacter::Jump);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->PrimaryAttackAction,ETriggerEvent::Triggered,this, &AARLCharacter::UsePrimaryAttack);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->SpecialAbilityAction,ETriggerEvent::Triggered,this, &AARLCharacter::UseSpecialAbility);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->MovementAbilityAction,ETriggerEvent::Triggered,this, &AARLCharacter::UseMovementAbility);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->InteractAction,ETriggerEvent::Triggered,this, &AARLCharacter::Interact);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->SprintAction,ETriggerEvent::Started,this, &AARLCharacter::StartSprint);
		EnhancedInputComponent->BindAction(InputActionsDataAsset->SprintAction,ETriggerEvent::Completed,this, &AARLCharacter::StopSprint);
	}
}


