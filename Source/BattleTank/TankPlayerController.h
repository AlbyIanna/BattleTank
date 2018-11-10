// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

#define OUT

// Forward declarations
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ATank* GetControlledTank() const;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float CrossHairLocationX = .5f;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float CrossHairLocationY = .33333f;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float LineTraceRange = 1000000.f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void AimTowardCrosshair();
	virtual bool GetSightRayHitLocation(OUT FVector & OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, OUT FVector & LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, OUT FVector & HitLocation) const;
};
