#pragma once

#include "GridActor.h"

//A laser grid actor that power ups other grid actors when its laser is pointing to it. Common puzzle game actor.
class LaserPowerUpGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(LaserPowerUpGridActor);

	LaserPowerUpGridActor();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void PowerUpHitRaycastActor();
	void PowerDownPreviousHitActor();

	class Polyboard* laser = nullptr;
	GridActor* previousHitGridActor = nullptr;
};
