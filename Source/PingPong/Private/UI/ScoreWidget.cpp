// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreWidget.h"
#include "PingPongGameState.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "BoardPlayerController.h"

void UScoreWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APingPongGameState* GameState = GetWorld()->GetGameState<APingPongGameState>();
	if (IsValid(GameState))
	{
		GameState->OnPlayerScoresChanged.AddUObject(this, &UScoreWidget::OnScoresUpdate);
	}
	else
	{
		ABoardPlayerController* PlayerController = GetOwningPlayer<ABoardPlayerController>();
		if (IsValid(PlayerController))
		{
			PlayerController->OnGameStateReady.AddWeakLambda(this, [this]()
				{
					BindToScoresUpdate();
				});

			PlayerController->OnPlayerStateReplicated.AddWeakLambda(this, [this]()
				{
					BindToScoresUpdate();
				});
		}
	}
}

void UScoreWidget::OnScoresUpdate(const TArray<FPlayerScore>& NewPlayerScores)
{
	for (const FPlayerScore& PlayerScore : NewPlayerScores)
	{
		const FText& Score = FText::FromString(FString::FromInt(PlayerScore.PlayerScore));
		if (PlayerScore.PlayerState == GetOwningPlayerState())
		{
			OwnScoreText->SetText(Score);
		}
		else
		{
			OpponentScoreText->SetText(Score);
		}
	}
}

void UScoreWidget::BindToScoresUpdate()
{
	APingPongGameState* GameState = GetWorld()->GetGameState<APingPongGameState>();
	if (IsValid(GameState))
	{
		if (!GameState->OnPlayerScoresChanged.IsBoundToObject(this))
		{
			GameState->OnPlayerScoresChanged.AddUObject(this, &UScoreWidget::OnScoresUpdate);
		}
	}
}
