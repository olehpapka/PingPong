// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Board.h"
#include "Net/UnrealNetwork.h"
#include "PingPong/PingPong.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "GameElements/Ball.h"
#include "PingPongGameState.h"
#include "PingPongGameMode.h"

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
	BoardMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GoalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalBox"));
	GoalBox->SetupAttachment(RootComponent);

	bReplicates = true;
}

void ABoard::Move(EMoveDirection Direction)
{
	if (!HasAuthority())
	{
		return;
	}

	if (Direction == EMoveDirection::Left)
	{
		BoardPosition += GetActorRightVector().GetSafeNormal() * MoveStep * -1;
	}
	else if (Direction == EMoveDirection::Right)
	{
		BoardPosition += GetActorRightVector().GetSafeNormal() * MoveStep;
	}

	SetActorLocation(BoardPosition, true);
}

void ABoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoard, BoardPosition);
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoardPosition = GetActorLocation();
		GoalBox->OnComponentBeginOverlap.AddDynamic(this, &ABoard::OnGoalBoxOverlap);
	}
}

void ABoard::OnRep_BoardPosition()
{
	SetActorLocation(BoardPosition, true);
}

void ABoard::OnGoalBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABall* Ball = Cast<ABall>(OtherActor);
	if (IsValid(Ball) && IsValid(GetWorld()))
	{
		APingPongGameState* GameState = GetWorld()->GetGameState<APingPongGameState>();
		if (IsValid(GameState))
		{
			GameState->PlayerMissedGoal(GetPlayerState());
		}
		APingPongGameMode* GameMode = GetWorld()->GetAuthGameMode<APingPongGameMode>();
		if (IsValid(GameMode))
		{
			GameMode->ResetBall();
		}
	}
}