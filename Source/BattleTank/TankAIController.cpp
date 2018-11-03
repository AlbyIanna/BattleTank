// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankPlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("AIController not passessing any tank"))
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AIController possessing: %s"), *(ControlledTank->GetName()))
		auto AimedTank = GetPlayerTank();
		if (!AimedTank) {
			UE_LOG(LogTemp, Warning, TEXT("Tank %s not aiming any tank"), *(ControlledTank->GetName()))
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at %s"), *(ControlledTank->GetName()), *(AimedTank->GetName()))
		}
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardPlayer();
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		return Cast<ATank>(PlayerController->GetPawn());
	}
	return nullptr;
}

void ATankAIController::AimTowardPlayer()
{
	GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
}
