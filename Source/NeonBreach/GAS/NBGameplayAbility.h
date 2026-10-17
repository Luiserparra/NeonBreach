// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NBGameplayAbility.generated.h"

/** Base gameplay ability. Concrete GAs land in M3+. */
UCLASS(Blueprintable)
class NEONBREACH_API UNBGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
};
