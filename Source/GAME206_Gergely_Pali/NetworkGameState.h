// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"
#include "NetworkGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameWon);
UCLASS()
class GAME206_GERGELY_PALI_API ANetworkGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void AddScore(APlayerState* ScoredPlayerState);

	float GetPlayerScore(APlayerState* PlayerState);

	FString GetWinningPlayer();

	FGameWon GameWon;
private:
	UPROPERTY(Replicated)
	int32 TargetDestroyedNum = 0;

	int32 MaxScore;

};
