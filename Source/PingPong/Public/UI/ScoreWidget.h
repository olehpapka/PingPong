// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PingPong/PingPong.h"
#include "ScoreWidget.generated.h"

class UTextBlock;

UCLASS()
class PINGPONG_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OwnScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OpponentScoreText;

	void OnScoresUpdate(const TArray<FPlayerScore>& NewPlayerScores);

	void BindToScoresUpdate();
};
