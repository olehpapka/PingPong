// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"
#include "Net/UnrealNetwork.h"

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongGameState, PlayerScores);
}

void APingPongGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (!HasAuthority())
	{
		return;
	}

	PlayerScores.Add(FPlayerScore{ PlayerState, 0 });
}

void APingPongGameState::PlayerMissedGoal(APlayerState* PlayerState)
{
	if (!HasAuthority())
	{
		return;
	}

	for (FPlayerScore& PlayerScore : PlayerScores)
	{
		if (PlayerScore.PlayerState != PlayerState)
		{
			PlayerScore.PlayerScore++;
			break;
		}
	}
}

void APingPongGameState::OnRep_PlayerScores()
{
	OnPlayerScoresChanged.Broadcast(PlayerScores);
}