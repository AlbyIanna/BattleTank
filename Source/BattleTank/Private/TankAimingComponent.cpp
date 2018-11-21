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
}

FVector UTankAimingComponent::GetAiminingHitLocation()
{
	return AimingHitLocation;
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
	UE_LOG(LogTemp, Warning, TEXT("Last: %d, reload: %f"), (FPlatformTime::Seconds() - LastFireTime), ReloadTimeInSeconds)
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds ) {
		FiringState = EFiringStatus::Reloading;
		UE_LOG(LogTemp, Warning, TEXT("Reloading"))
	}
	else if (isBarrelMoving()) {
		FiringState = EFiringStatus::Aiming;
		UE_LOG(LogTemp, Warning, TEXT("Aiming"))
	}
	else {
		FiringState = EFiringStatus::Locked;
		UE_LOG(LogTemp, Warning, TEXT("Locked"))
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
		auto Time = GetWorld()->GetTimeSeconds();
		AimingHitLocation = HitLocation;
	}
	else {
		auto Time = GetWorld()->GetTimeSeconds();
		AimingHitLocation = FVector();
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector NewAimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = NewAimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

bool UTankAimingComponent::isBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	return !AimDirection.Equals(Barrel->GetForwardVector(), .01);
}

void UTankAimingComponent::Fire()
{
	//FVector AimingHitLocation = TankAimingComponent->GetAiminingHitLocation();

	if (FiringState == EFiringStatus::Reloading || !ensure(Barrel) || !ensure(ProjectileBlueprint)) { return; }
	
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
		);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
}
