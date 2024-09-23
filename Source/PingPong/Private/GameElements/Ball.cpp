// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/Ball.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = false;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	bReplicates = true;
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

	if (HasAuthority())
	{
		BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnBallHit);
	}
}

void ABall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector CurrentVelocity = ProjectileMovement->Velocity;

	FVector ReflectedVelocity = CurrentVelocity.MirrorByVector(Hit.ImpactNormal);

	float Speed = CurrentVelocity.Size();
	ProjectileMovement->Velocity = ReflectedVelocity.GetSafeNormal() * Speed;
}
