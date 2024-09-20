// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTPPProjectGameMode.h"
#include "MyTPPProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyTPPProjectGameMode::AMyTPPProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		//DefaultPawnClass = AMyTPPProjectCharacter::StaticClass();
	}
}
