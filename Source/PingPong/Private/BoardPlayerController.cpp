// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PingPong/PingPong.h"
#include "GameElements/Board.h"
#include "Blueprint/UserWidget.h"

void ABoardPlayerController::Client_WaitForStart_Implementation()
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(this, WaitingWidget);
	if (IsValid(Widget))
	{
		Widget->AddToViewport();
		CurrentWidget = Widget;
	}
}

void ABoardPlayerController::Client_ShowInProgressWidget_Implementation()
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromParent();
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(this, InProgressWidget);
	if (IsValid(Widget))
	{
		Widget->AddToViewport();
		CurrentWidget = Widget;
	}
}

void ABoardPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReplicated.Broadcast();
}

void ABoardPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ABoardPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ABoardPlayerController::MoveInternal, EMoveDirection::Left);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ABoardPlayerController::MoveInternal, EMoveDirection::Right);
	}
}

void ABoardPlayerController::MoveInternal(const FInputActionValue& Value, EMoveDirection Direction)
{
	Server_MoveBoard(Direction);
}

void ABoardPlayerController::Server_MoveBoard_Implementation(EMoveDirection Direction)
{
	ABoard* Board = Cast<ABoard>(GetPawn());
	if (IsValid(Board))
	{
		Board->Move(Direction);
	}
}
