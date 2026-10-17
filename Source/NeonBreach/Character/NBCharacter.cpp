// Copyright Epic Games, Inc. All Rights Reserved.

#include "NBCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "NeonBreach.h"

ANBCharacter::ANBCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// TPS shooter: yaw follows look; strafe relative to camera (not template orbit).
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ANBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogNeonBreach, Error, TEXT("'%s' Failed to find an Enhanced Input component."), *GetNameSafe(this));
		return;
	}

	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANBCharacter::Move);
	}
	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANBCharacter::Look);
	}
	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	if (FireAction)
	{
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ANBCharacter::OnFire);
	}
	if (Ability1Action)
	{
		EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Started, this, &ANBCharacter::OnAbility1);
	}
	if (Ability2Action)
	{
		EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Started, this, &ANBCharacter::OnAbility2);
	}
	if (Ability3Action)
	{
		EnhancedInputComponent->BindAction(Ability3Action, ETriggerEvent::Started, this, &ANBCharacter::OnAbility3);
	}
	if (ScoreboardAction)
	{
		EnhancedInputComponent->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &ANBCharacter::OnScoreboard);
	}
}

void ANBCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ANBCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ANBCharacter::DoMove(float Right, float Forward)
{
	if (GetController() == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void ANBCharacter::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void ANBCharacter::OnFire(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogNeonBreach, Verbose, TEXT("%s Fire (stub)"), *GetNameSafe(this));
}

void ANBCharacter::OnAbility1(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogNeonBreach, Verbose, TEXT("%s Ability1 (stub)"), *GetNameSafe(this));
}

void ANBCharacter::OnAbility2(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogNeonBreach, Verbose, TEXT("%s Ability2 (stub)"), *GetNameSafe(this));
}

void ANBCharacter::OnAbility3(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogNeonBreach, Verbose, TEXT("%s Ability3 (stub)"), *GetNameSafe(this));
}

void ANBCharacter::OnScoreboard(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogNeonBreach, Verbose, TEXT("%s Scoreboard (stub)"), *GetNameSafe(this));
}
