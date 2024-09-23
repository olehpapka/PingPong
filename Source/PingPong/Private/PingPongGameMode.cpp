// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "BoardPlayerContoller.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameElements/Ball.h"

AActor* APingPongGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const int32 PlayerStartsNum = PlayerStarts.Num();
	APlayerStart* PlayerStart = nullptr;
	if (PlayerStartsNum != 0)
	{
		PlayerStart = PlayerStarts[PlayerStartsNum - 1];
		PlayerStarts.RemoveAt(PlayerStartsNum - 1);
	}

	return IsValid(PlayerStart) ? PlayerStart : Super::ChoosePlayerStart_Implementation(Player);
}

void APingPongGameMode::ResetBall()
{
	if (IsValid(Ball))
	{
		const FTransform& BallTransform = GetBallSpawnTransform();
		Ball->SetActorLocation(BallTransform.GetLocation());
		Ball->SetActorRotation(BallTransform.GetRotation().Rotator());
		Ball->ResetVelocity();
	}
}

void APingPongGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> PlayerStartsActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStartsActors);

	for (AActor* PlayerStartActor : PlayerStartsActors)
	{
		PlayerStarts.Emplace(Cast<APlayerStart>(PlayerStartActor));
	}
}

void APingPongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NumPlayers == 1)
	{
		ABoardPlayerContoller* BoardPlayerController = Cast<ABoardPlayerContoller>(NewPlayer);
		if (IsValid(BoardPlayerController))
		{
			BoardPlayerController->Client_WaitForStart();
		}
	}
	else if (NumPlayers == 2)
	{
		if (IsValid(GameState))
		{
			for (APlayerState* PlayerState : GameState->PlayerArray)
			{
				ABoardPlayerContoller* BoardPlayerController = Cast<ABoardPlayerContoller>(PlayerState->GetOwner());
				if (IsValid(BoardPlayerController))
				{
					BoardPlayerController->Client_ShowInProgressWidget();
				}
			}
		}

		SpawnBall();
	}
}

void APingPongGameMode::SpawnBall()
{	
	Ball = GetWorld()->SpawnActor<ABall>(BallClass, GetBallSpawnTransform());
}

FTransform APingPongGameMode::GetBallSpawnTransform()
{
	FTransform SpawnTransform;
	if (IsValid(BallSpawnPoint))
	{
		SpawnTransform = BallSpawnPoint->GetActorTransform();
	}
	else
	{
		BallSpawnPoint = UGameplayStatics::GetActorOfClass(this, BallSpawnPointClass);
		if (!IsValid(BallSpawnPoint))
		{
			return FTransform();
		}

		SpawnTransform = BallSpawnPoint->GetActorTransform();
	}

	FRotator Rotation;
	Rotation.Pitch = 0;
	Rotation.Roll = 0;

	if (FMath::RandBool())
	{
		Rotation.Yaw = FMath::RandRange(BallSpawnForwardRotation.X, BallSpawnForwardRotation.Y);
	}
	else
	{
		Rotation.Yaw = FMath::RandRange(BallSpawnBackwardRotation.X, BallSpawnBackwardRotation.Y);
	}

	if (Rotation.Yaw > 180.0)
	{
		Rotation.Yaw -= 360.0;
	}

	SpawnTransform.SetRotation(Rotation.Quaternion());
	return SpawnTransform;
}
