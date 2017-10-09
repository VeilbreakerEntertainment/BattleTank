// (c) Copyright 2017 Veilbreaker Entertainment

#include "TankPlayerController.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();

	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is not possesing a tank."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is possessing tank: %s"), *(ControlledTank->GetName()));
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation)) // Has "side-effect", is going to line trace
	{
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

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLoc,
			EndLoc,
			ECollisionChannel::ECC_Visibility,
			Params
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