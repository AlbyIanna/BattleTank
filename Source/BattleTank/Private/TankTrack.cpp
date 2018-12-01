// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TankTrack.h"
#include "Engine/World.h"
#include <string>         

UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::SetThrottle(float Throttle) {
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::DriveTrack() {
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;

}

void UTankTrack::ApplySidewaysForce() {
	// calculate the slippage speed
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// workout the required acceleration this frame to correct
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorretionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	
	// calcaulate and apply sideways for (F = m a )
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = (TankRoot->GetMass() * CorretionAcceleration) / 2;
	TankRoot->AddForce(CorrectionForce);
}