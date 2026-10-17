// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NBCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * NeonBreach third-person pawn. Camera + Enhanced Input live here.
 * Mesh and Anim BP are assigned on BP_NBCharacter.
 */
UCLASS(abstract)
class NEONBREACH_API ANBCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

protected:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> Ability1Action;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> Ability2Action;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> Ability3Action;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ScoreboardAction;

public:

	ANBCharacter();

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void OnFire(const FInputActionValue& Value);
	void OnAbility1(const FInputActionValue& Value);
	void OnAbility2(const FInputActionValue& Value);
	void OnAbility3(const FInputActionValue& Value);
	void OnScoreboard(const FInputActionValue& Value);

public:

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
