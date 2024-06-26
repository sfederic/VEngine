#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Crystal that powers up surrounding apparatus in area of effect.
class PowerCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(PowerCrystal);

	PowerCrystal();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void PowerUpSurroundingGridActors();

	BoxTriggerComponent* effectTrigger = nullptr;
};
