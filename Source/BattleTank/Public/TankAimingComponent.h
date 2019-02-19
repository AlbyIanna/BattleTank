// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringStatus : uint8
{
	Aiming,
	Reloading,
	Locked,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel * BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire(); // _FIRE_

	void AimAt(FVector HitLocation);

	FVector GetAiminingHitLocation();

	EFiringStatus getFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 getCurrentAmmo() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringState = EFiringStatus::Reloading;

	
private:
	// Sets default values for this component's properties
	UTankAimingComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	FVector AimingHitLocation = FVector();
	FVector AimDirection = FVector();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint; // Alternative: SubcalssOf

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 5000;
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 maxAmmo = 3;

	int32 currentAmmo;
	double LastFireTime = 0;

	void MoveBarrelTowards(FVector NewAimDirection);
	bool isBarrelMoving();
	void refillAmmo();
};
