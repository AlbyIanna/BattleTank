// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

#define OUT

// Forward declarations
class UTankAimingComponent;

/**
 * Responsible for helping the player aim
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingComponentRed);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float CrossHairLocationX = .5f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float CrossHairLocationY = .33333f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LineTraceRange = 1000000.f;

	UTankAimingComponent * AimingComponent = nullptr;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void AimTowardCrosshair();
	virtual bool GetSightRayHitLocation(OUT FVector & OutHitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, OUT FVector & LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, OUT FVector & HitLocation) const;
};
