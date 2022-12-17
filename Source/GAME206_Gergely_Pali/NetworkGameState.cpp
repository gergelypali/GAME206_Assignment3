// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameState.h"
#include "GameFramework/PlayerState.h"
#include "Target.h"
#include "GAME206_Gergely_PaliGameMode.h"

void ANetworkGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkGameState, TargetDestroyedNum);
}

void ANetworkGameState::AddScore(APlayerState* ScoredPlayerState)
{
	ScoredPlayerState->SetScore(ScoredPlayerState->GetScore() + 1);
	TargetDestroyedNum++;

	if (TargetDestroyedNum == Cast<AGAME206_Gergely_PaliGameMode>(GetWorld()->GetAuthGameMode())->MaxNumOfTarget)
	{

		GameWon.Broadcast();
	}
}

float ANetworkGameState::GetPlayerScore(APlayerState* PlayerState)
{
	return PlayerState->GetScore();
}

FString ANetworkGameState::GetWinningPlayer()
{
	APlayerState* winningPlayer = PlayerArray[0];
	for (APlayerState* PS : PlayerArray)
	{
		if (PS->GetScore() > winningPlayer->GetScore())
		{
			winningPlayer = PS;
		}
	}
	return winningPlayer->GetName();
}
