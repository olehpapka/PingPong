// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "BoardPlayerController.h"

void APingPongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongGameState, PlayerScores);
}

void APingPongGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (HasAuthority())
	{
		PlayerScores.Add(FPlayerScore{ PlayerState, 0 });
	}
	else
	{
		ABoardPlayerController* PlayerController = Cast<ABoardPlayerController>(PlayerState->GetPlayerController());
		if (IsValid(PlayerController))
		{
			PlayerController->OnGameStateReady.Broadcast();
		}
	}
	
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

void APingPongGameState::BeginPlay()
{
	Super::BeginPlay();

	//if (!HasAuthority())
	//{
	//	for (APlayerState* PlayerState : PlayerArray)
	//	{
	//		if (!IsValid(PlayerState))
	//		{
	//			continue;
	//		}

	//		ABoardPlayerController* PlayerController = Cast<ABoardPlayerController>(PlayerState->GetPlayerController());
	//		if (IsValid(PlayerController))
	//		{
	//			PlayerController->OnGameStateReplicated.Broadcast();
	//		}
	//	}
	//}
}

void APingPongGameState::OnRep_PlayerScores()
{
	OnPlayerScoresChanged.Broadcast(PlayerScores);
}