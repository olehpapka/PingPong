// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoardPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

enum class EMoveDirection : uint8;

DECLARE_MULTICAST_DELEGATE(FOnGameStateReady);
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateReplicated);

UCLASS()
class PINGPONG_API ABoardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void Client_WaitForStart();

	UFUNCTION(Client, Reliable)
	void Client_ShowInProgressWidget();

	virtual void OnRep_PlayerState() override;

	FOnGameStateReady OnGameStateReady;
	FOnPlayerStateReplicated OnPlayerStateReplicated;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable)
	void Server_MoveBoard(EMoveDirection Direction);

	void MoveInternal(const FInputActionValue& Value, EMoveDirection Direction);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> WaitingWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InProgressWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;
};
