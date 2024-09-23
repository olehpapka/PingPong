// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPlayerContoller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PingPong/PingPong.h"
#include "GameElements/Board.h"
#include "Blueprint/UserWidget.h"

void ABoardPlayerContoller::Client_WaitForStart_Implementation()
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(this, WaitingWidget);
	if (IsValid(Widget))
	{
		Widget->AddToViewport();
		CurrentWidget = Widget;
	}
}

void ABoardPlayerContoller::Client_ShowInProgressWidget_Implementation()
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

void ABoardPlayerContoller::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReplicated.Broadcast();
}

void ABoardPlayerContoller::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ABoardPlayerContoller::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ABoardPlayerContoller::MoveInternal, EMoveDirection::Left);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ABoardPlayerContoller::MoveInternal, EMoveDirection::Right);
	}
}

void ABoardPlayerContoller::MoveInternal(const FInputActionValue& Value, EMoveDirection Direction)
{
	Server_MoveBoard(Direction);
}

void ABoardPlayerContoller::Server_MoveBoard_Implementation(EMoveDirection Direction)
{
	ABoard* Board = Cast<ABoard>(GetPawn());
	if (IsValid(Board))
	{
		Board->Move(Direction);
	}
}
