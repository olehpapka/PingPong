// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Board.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;

enum class EMoveDirection : uint8;

UCLASS()
class PINGPONG_API ABoard : public APawn
{
	GENERATED_BODY()

public:
	ABoard();

	void Move(EMoveDirection Direction);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_BoardPosition)
	FVector BoardPosition;

	UFUNCTION()
	void OnRep_BoardPosition();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BoardMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> GoalBox;

	UPROPERTY(EditDefaultsOnly, Category = "Board|Settings")
	float MoveStep = 10.0f;

	UFUNCTION()
	void OnGoalBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
