// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Public/TankBarrel.h"
#include "Public/TankTurret.h"
#include "Public/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
	currentAmmo = maxAmmo;
}

FVector UTankAimingComponent::GetAiminingHitLocation()
{
	return AimingHitLocation;
}

EFiringStatus UTankAimingComponent::getFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::getCurrentAmmo() const
{
	return currentAmmo;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (currentAmmo <= 0) {
		FiringState = EFiringStatus::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds ) {
		FiringState = EFiringStatus::Reloading;
	}
	else if (isBarrelMoving()) {
		FiringState = EFiringStatus::Aiming;
	}
	else {
		FiringState = EFiringStatus::Locked;
	}
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }
	FVector OutLaunchVelocity = FVector(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // HighArc = false
		0, // CollisionRadius = 0 -> Aim precisely
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	)) {
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		AimingHitLocation = HitLocation;
	}
	else {
		AimingHitLocation = FVector();
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector NewAimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = NewAimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	if (FMath::Abs(DeltaRotator.Yaw) > 180) { // Always yaw the shortest way
		DeltaRotator.Yaw = -DeltaRotator.Yaw;
	}

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

bool UTankAimingComponent::isBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	return !AimDirection.Equals(Barrel->GetForwardVector(), .01);
}

void UTankAimingComponent::refillAmmo()
{
	currentAmmo = maxAmmo;
}

void UTankAimingComponent::Fire()
{
	//FVector AimingHitLocation = TankAimingComponent->GetAiminingHitLocation();

	if (FiringState == EFiringStatus::Reloading || FiringState == EFiringStatus::OutOfAmmo || !ensure(Barrel) || !ensure(ProjectileBlueprint)) { return; }
	
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
		);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
	currentAmmo--;
}
