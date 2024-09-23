// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PingPong.generated.h"

USTRUCT()
struct FPlayerScore
{
	GENERATED_BODY();

	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY()
	int32 PlayerScore = 0;
};

UENUM()
enum class EMoveDirection : uint8
{
	None,
	Left,
	Right,
};
