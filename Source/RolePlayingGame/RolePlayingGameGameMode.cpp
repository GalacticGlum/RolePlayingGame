// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RolePlayingGameGameMode.h"
#include "RolePlayingGameCharacter.h"
#include "RolePlayingGamePlayerController.h"
#include "UObject/ConstructorHelpers.h"

ARolePlayingGameGameMode::ARolePlayingGameGameMode()
{
	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> playerPawnBPClass(TEXT("/Game/Blueprints/CHAR_Player"));
	if (playerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = playerPawnBPClass.Class;
	}

	// Set the player controller class to our custom player controller
	PlayerControllerClass = ARolePlayingGamePlayerController::StaticClass();
}
