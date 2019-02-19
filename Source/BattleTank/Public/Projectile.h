// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// Forward declarations
class UProjectileMovementComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent * CollisionMesh, UParticleSystemComponent * LaunchBlast);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent * CollisionMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent * LaunchBlast = nullptr;

	UProjectileMovementComponent* ProjectileMovement = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchProjectile(float Speed);
	
};
