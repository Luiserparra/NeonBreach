// Copyright Epic Games, Inc. All Rights Reserved.

#include "NBGameMode.h"
#include "NBGameState.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"

ANBGameMode::ANBGameMode()
{
	PlayerControllerClass = ANBPlayerController::StaticClass();
	PlayerStateClass = ANBPlayerState::StaticClass();
	GameStateClass = ANBGameState::StaticClass();
}
