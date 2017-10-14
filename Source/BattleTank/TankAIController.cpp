// (c) Copyright 2017 Veilbreaker Entertainment

#include "TankAIController.h"
#include "TankAimingComponent.h"
// Depends on movement component via path-finding system

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

	//Fire if ready
	AimingComponent->Fire();
}