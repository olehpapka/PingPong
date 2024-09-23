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

	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
	RootComponent = BoardMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoardMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GoalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalBox"));
	GoalBox->SetupAttachment(BoardMesh);

	bReplicates = true;
}

void ABoard::Move(EMoveDirection Direction)
{
	if (!HasAuthority())
	{
		return;
	}

	int32 SignModifier = 0;
	if (Direction == EMoveDirection::Left)
	{
		SignModifier = -1;
	}
	else if (Direction == EMoveDirection::Right)
	{
		SignModifier = 1;
	}

	FVector DeltaToAdd = GetActorRightVector().GetSafeNormal() * MoveStep * SignModifier;
	
	bool bResult = SetActorLocation(BoardPosition + DeltaToAdd, true);
	if (bResult)
	{
		BoardPosition += DeltaToAdd;
	}
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
	else
	{
		FLinearColor BoardColor;
		if (GetController())
		{
			BoardColor = FLinearColor::Green;
		}
		else
		{
			BoardColor = FLinearColor::Red;
		}

		UMaterialInstanceDynamic* DynMaterial = BoardMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (IsValid(DynMaterial))
		{
			DynMaterial->SetVectorParameterValue("Color", BoardColor);
		}
	}
}

void ABoard::OnRep_BoardPosition()
{
	SetActorLocation(BoardPosition);
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