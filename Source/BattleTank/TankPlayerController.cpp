/// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"


void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardCrosshair();
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}

void ATankPlayerController::AimTowardCrosshair()
{
	if (!ensure(AimingComponent) || !ensure(GetPawn())) { return; }

	FVector HitLocation;
	bool bGotHitLocation = GetSightRayHitLocation(OUT HitLocation);
	if (bGotHitLocation) {
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(OUT FVector & OutHitLocation) const
{
	/// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairLocationX, ViewportSizeY * CrossHairLocationY);
	
	/// De-project the crosshair screen position to a world direction
	FVector LookDirection; // To be discarded
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		return GetLookVectorHitLocation(LookDirection, OUT OutHitLocation);
	}
	return false;
	
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, OUT FVector & LookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, OUT CameraWorldLocation, OUT LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, OUT FVector & HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	if (
		GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
		)
	) {
		HitLocation = HitResult.Location;
		return true;
	}
	else {
		HitLocation = FVector(0);
		return false;
	}
}

void ATankPlayerController::OnTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("my tank is dead! D:"))

	StartSpectatingOnly();
}
