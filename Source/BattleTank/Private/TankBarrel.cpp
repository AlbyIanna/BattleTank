// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TankBarrel.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

void UTankBarrel::Elevate(float RawRelativeSpeed)
{
	auto RelativeSpeed = FMath::Clamp(RawRelativeSpeed, -1.f, 1.f);
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	auto NewElevation = FMath::Clamp(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(NewElevation, 0, 0));
}
