// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreWidget.h"
#include "PingPongGameState.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "BoardPlayerContoller.h"

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
		ABoardPlayerContoller* PlayerController = GetOwningPlayer<ABoardPlayerContoller>();
		if (IsValid(PlayerController))
		{
			PlayerController->OnPlayerStateReplicated.AddWeakLambda(this, [this]() 
				{
					APingPongGameState* GameState = GetWorld()->GetGameState<APingPongGameState>();
					if (IsValid(GameState))
					{
						GameState->OnPlayerScoresChanged.AddUObject(this, &UScoreWidget::OnScoresUpdate);
					}
				});
		}
	}
}

void UScoreWidget::OnScoresUpdate(const TArray<FPlayerScore>& NewPlayerScores)
{
	for (const FPlayerScore& PlayerScore : NewPlayerScores)
	{
		if (PlayerScore.PlayerState == GetOwningPlayerState())
		{
			OwnScoreText->SetText(FText::FromString(FString::FromInt(PlayerScore.PlayerScore)));
		}
		else
		{
			OpponentScoreText->SetText(FText::FromString(FString::FromInt(PlayerScore.PlayerScore)));
		}
	}
}
