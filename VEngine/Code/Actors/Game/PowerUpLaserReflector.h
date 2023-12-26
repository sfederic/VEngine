#pragma once

#include "GridActor.h"

//Reflects a powerup laser. You know the puzzle cliche.
class PowerUpLaserReflector : public GridActor
{
public:
	ACTOR_SYSTEM(PowerUpLaserReflector);

	PowerUpLaserReflector();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;

	void PowerUp() override;
	void PowerDown() override;

private:
	void ReflectIncomingLaser();

	class Polyboard* reflectedLaser = nullptr;
};
