// (c) Copyright 2017 Veilbreaker Entertainment

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	auto Name = GetName();

	UE_LOG(LogTemp, Warning, TEXT("%s throttle: %f"), *Name, Throttle);
}