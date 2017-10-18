// (c) Copyright 2017 Veilbreaker Entertainment

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent)) { return; }

	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto ControlledTank = Cast<ATank>(InPawn);

		if (!ensure(ControlledTank)) { return; }

		ControlledTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnControlledTankDeath);
	}
}

void ATankPlayerController::OnControlledTankDeath()
{

}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (GetPawn())
	{
		auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
		FVector HitLocation; // Out parameter

		if (!ensure(AimingComponent)) { return; }

		if (GetSightRayHitLocation(HitLocation)) // Has "side-effect", is going to line trace
		{
			AimingComponent->AimAt(HitLocation);
		}
	}
}

//Line-trace through the cross-hair
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//Find the cross-hair position
	int32 ViewportSizeX, ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLoc = FVector2D(ViewportSizeX * CrosshairXLoc, ViewportSizeY * CrosshairYLoc);
	FVector LookDirection;

	if (GetLookDirection(ScreenLoc, LookDirection))
	{
		//Line-trace single by channel (returns a bool)
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
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
	//Convert position of cross-hair to a world direction
	FVector CameraWorldLocation; //To be discarded

	return DeprojectScreenPositionToWorld(
		ScreenLoc.X, ScreenLoc.Y, 
		CameraWorldLocation, 
		LookDirection
	);
}