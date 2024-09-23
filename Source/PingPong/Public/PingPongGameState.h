// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PingPong/PingPong.h"
#include "PingPongGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerScoresChanged, const TArray<FPlayerScore>&);

UCLASS()
class PINGPONG_API APingPongGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	void PlayerMissedGoal(APlayerState* PlayerState);

	FOnPlayerScoresChanged OnPlayerScoresChanged;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerScores)
	TArray<FPlayerScore> PlayerScores;

	UFUNCTION()
	void OnRep_PlayerScores();
};
