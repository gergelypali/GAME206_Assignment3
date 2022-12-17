// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME206_Gergely_PaliGameMode.h"
#include "GAME206_Gergely_PaliCharacter.h"
#include "CountdownSceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Target.h"
#include "NetworkGameState.h"

AGAME206_Gergely_PaliGameMode::AGAME206_Gergely_PaliGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	GameStateClass = ANetworkGameState::StaticClass();
}

void AGAME206_Gergely_PaliGameMode::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void AGAME206_Gergely_PaliGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!NewPlayer->NetConnection)
	{
		Cast<AGAME206_Gergely_PaliCharacter>(NewPlayer->GetCharacter())->SpawnNewTarget();
	}
}
