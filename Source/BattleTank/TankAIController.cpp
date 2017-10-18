// (c) Copyright 2017 Veilbreaker Entertainment

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
// Depends on movement component via path-finding system

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto ControlledTank = Cast<ATank>(InPawn);

		if (!ensure(ControlledTank)) { return; }

		ControlledTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnControlledTankDeath);
	}
}

void ATankAIController::OnControlledTankDeath()
{
	if (!GetPawn()) { return; }

	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	if (!ensure(AimingComponent)) { return; }

	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);

	//Aim at the player
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// AI only fires if the aim state is locked
	if (AimingComponent->GetAimState() == EAimState::Locked)
	{
		AimingComponent->Fire();
	}
}