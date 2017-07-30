// (c) Copyright 2017 Veilbreaker Entertainment

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AimAtCrosshair();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimAtCrosshair()
{
	FVector HitLocation; //Out parameter

	if (!GetControlledTank()) { return; }

	if (GetSightRayHitLocation(HitLocation))
	{
		//Tell controlled tank to aim at this location
		GetControlledTank()->AimAt(HitLocation);
	}
}

//Line-trace through the crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLoc = FVector2D(ViewportSizeX * CrosshairXLoc, ViewportSizeY * CrosshairYLoc);
	FVector LookDirection;

	if (GetLookDirection(ScreenLoc, LookDirection))
	{
		//Line-trace single by channel
		GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLoc = PlayerCameraManager->GetCameraLocation();
	auto EndLoc = StartLoc + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLoc,
			EndLoc,
			ECollisionChannel::ECC_Visibility
		))
	{
		HitLocation = HitResult.Location;

		return true;
	}
	HitLocation = FVector(0);

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLoc, FVector& LookDirection) const
{
	//Convert position of crosshair to a world direction
	FVector CameraWorldLocation; //To be discarded

	return DeprojectScreenPositionToWorld(
		ScreenLoc.X, ScreenLoc.Y, 
		CameraWorldLocation, 
		LookDirection
	);
}