// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PingPongGameMode.generated.h"

class APlayerStart;
class ABall;

UCLASS()
class PINGPONG_API APingPongGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void ResetBall();

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	TArray<TObjectPtr<APlayerStart>> PlayerStarts;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BallClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BallSpawnPointClass;

	UPROPERTY()
	TObjectPtr<AActor> BallSpawnPoint;

	UPROPERTY()
	TObjectPtr<ABall> Ball;

	UPROPERTY(EditDefaultsOnly)
	FVector2D BallSpawnForwardRotation = FVector2D(-50.0, 50.0);

	UPROPERTY(EditDefaultsOnly)
	FVector2D BallSpawnBackwardRotation = FVector2D(220.0, 320.0);

	void SpawnBall();

	FTransform GetBallSpawnTransform();
};
