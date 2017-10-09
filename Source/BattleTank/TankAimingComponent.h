// (c) Copyright 2017 Veilbreaker Entertainment

#pragma once

#include "TankBarrel.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"
// Forward declarations
class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();

	void SetBarrelReference(UTankBarrel* BarrelToSet);
	void SetTurretReference(UTankTurret* TurretToSet);
	void AimAt(FVector HitLocation, float LaunchSpeed);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	void MoveBarrelTowards(FVector AimDirection);
};