// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Ball.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameElements/Board.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = false;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	bReplicates = true;
}

void ABall::NetMulticast_ResetBall_Implementation(const FTransform& NewBallTransform)
{
	SetActorLocation(NewBallTransform.GetLocation());
	SetActorRotation(NewBallTransform.GetRotation().Rotator());
	ResetVelocity();
}

void ABall::ResetVelocity()
{
	if (IsValid(ProjectileMovement))
	{
		const double Speed = ProjectileMovement->InitialSpeed;
		ProjectileMovement->Velocity = GetActorRotation().Vector().GetSafeNormal() * Speed;
	}
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnBallHit);
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(ProjectileMovement))
	{
		FVector CurrentVelocity = ProjectileMovement->Velocity;

		FVector ReflectedVelocity = CurrentVelocity.MirrorByVector(Hit.ImpactNormal);
		const double Speed = ProjectileMovement->InitialSpeed;

		ProjectileMovement->Velocity = ReflectedVelocity.GetSafeNormal() * Speed;
	}
	
	if (HasAuthority())
	{
		ABoard* Board = Cast<ABoard>(OtherActor);
		if (IsValid(Board))
		{
			BallTouchedByPlayer.ExecuteIfBound();
		}
	}
}
