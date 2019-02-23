// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Tank.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerTank) || !ensure(AimingComponent)) { return; }
	
	MoveToActor(PlayerTank, AcceptanceRadius);
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	
	if (AimingComponent->getFiringState() == EFiringStatus::Locked) {
		AimingComponent->Fire();
	}
}

void ATankAIController::OnTankDeath()
{ 
	UE_LOG(LogTemp, Warning, TEXT("Enemy tank %s IS DEAD! :D"), *GetPawn()->GetName())

	auto PossessedTank = GetPawn();
	if (!ensure(PossessedTank)) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

