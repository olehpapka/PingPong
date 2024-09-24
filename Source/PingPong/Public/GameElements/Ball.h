// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class UProjectileMovementComponent;

DECLARE_DELEGATE(FBallTouchedByPlayer);

UCLASS()
class PINGPONG_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABall();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ResetBall(const FTransform& NewBallTransform);

	void ResetVelocity();

	FBallTouchedByPlayer BallTouchedByPlayer;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BallMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
