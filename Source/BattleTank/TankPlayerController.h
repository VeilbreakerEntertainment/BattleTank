// (c) Copyright 2017 Veilbreaker Entertainment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// Forward declarations
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	ATank* GetControlledTank() const;

	// Start the tank moving the barrel so that a shot would hit where the crosshair is
	void AimTowardsCrosshair();

	//Return an Out parameter, true if hit landscape.
	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLoc, FVector& LookDirection) const;

	UPROPERTY(EditAnywhere)
	float CrosshairXLoc = 0.5;

	UPROPERTY(EditAnywhere)
	float CrosshairYLoc = 0.3333;

	UPROPERTY(EditAnywhere)
	float LineTraceRange = 10000;
};