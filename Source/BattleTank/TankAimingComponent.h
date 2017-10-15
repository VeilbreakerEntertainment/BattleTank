// (c) Copyright 2017 Veilbreaker Entertainment

#pragma once

#include "TankBarrel.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state
UENUM() // Adhering to Unreal coding standards
enum class EAimState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

// Forward declarations
class AProjectile;
class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	int GetAmmoLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	void AimAt(FVector HitLocation);

	EAimState GetAimState() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Aim State")
	EAimState AimState = EAimState::Reloading;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void MoveBarrelTowards(FVector AimDirection);

	bool IsBarrelMoving();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 8000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	FVector AimDirection;

	int AmmoLeft = 3;
};