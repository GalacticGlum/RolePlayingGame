// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RolePlayingGameGameMode.h"
#include "RolePlayingGameCharacter.h"
#include "RolePlayingGamePlayerController.h"
#include "UObject/ConstructorHelpers.h"

ARolePlayingGameGameMode::ARolePlayingGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ARolePlayingGamePlayerController::StaticClass();
}
