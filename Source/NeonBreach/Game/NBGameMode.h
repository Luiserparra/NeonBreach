// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameMode.generated.h"

/**
 * NeonBreach game mode. Default pawn is assigned on BP_NBGameMode
 * (ANBCharacter is abstract).
 */
UCLASS(Blueprintable)
class NEONBREACH_API ANBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ANBGameMode();
};
