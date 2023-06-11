#pragma once

#include "GridActor.h"

class PowerCrystalGear : public GridActor
{
public:
	ACTOR_SYSTEM(PowerCrystalGear);

	void Create() override;
	void Tick(float deltaTime) override {}
	void PowerUp() override;
};
