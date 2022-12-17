// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAME206_Gergely_PaliGameMode.generated.h"

UCLASS(minimalapi)
class AGAME206_Gergely_PaliGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAME206_Gergely_PaliGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxNumOfTarget = 11;

	void HandleTargetDied();

	void ExitGame();

	void PauseAndDisplayResult();

};



