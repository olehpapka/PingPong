// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "BoardPlayerController.h"
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
		Ball->NetMulticast_ResetBall(GetBallSpawnTransform());

		GetWorldTimerManager().SetTimer(BallUntouchedTimerHandle, this, &APingPongGameMode::ResetBall, BallUntouchedTime, true);
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
		ABoardPlayerController* BoardPlayerController = Cast<ABoardPlayerController>(NewPlayer);
		if (IsValid(BoardPlayerController))
		{
			BoardPlayerController->Client_WaitForStart();
		}
	}
	else if (NumPlayers == 2)
	{
		if (!IsValid(GameState))
		{
			return;
		}

		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			if (!IsValid(PlayerState))
			{
				continue;
			}

			ABoardPlayerController* BoardPlayerController = Cast<ABoardPlayerController>(PlayerState->GetOwner());
			if (IsValid(BoardPlayerController))
			{
				BoardPlayerController->Client_ShowInProgressWidget();
			}
		}

		SpawnBall();
	}
}

void APingPongGameMode::SpawnBall()
{
	Ball = GetWorld()->SpawnActor<ABall>(BallClass, GetBallSpawnTransform());
	if (IsValid(Ball))
	{
		Ball->BallTouchedByPlayer.BindWeakLambda(this, [this]()
			{
				GetWorldTimerManager().SetTimer(BallUntouchedTimerHandle, this, &APingPongGameMode::ResetBall, BallUntouchedTime, true);
			});
	}

	GetWorldTimerManager().SetTimer(BallUntouchedTimerHandle, this, &APingPongGameMode::ResetBall, BallUntouchedTime, true);
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

	FVector ForwardVector = BallSpawnPoint->GetActorForwardVector();

	FVector Direction;

	const double RandomYaw = FMath::RandRange(BallSpawnRotation.X, BallSpawnRotation.Y);
	if (FMath::RandBool())
	{
		Direction = ForwardVector.RotateAngleAxis(RandomYaw, FVector::UpVector);
	}
	else
	{
		Direction = -ForwardVector.RotateAngleAxis(RandomYaw, FVector::UpVector);
	}

	FRotator Rotation;
	Rotation.Pitch = 0;
	Rotation.Roll = 0;
	Rotation = Direction.Rotation();
	SpawnTransform.SetRotation(Rotation.Quaternion());
	return SpawnTransform;
}
