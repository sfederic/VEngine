#pragma once

#include "GridActor.h"

class Explosive : public GridActor
{
public:
	ACTOR_SYSTEM(Explosive);

	void Tick(float deltaTime) override;
	void Create() override;
	void Interact() override;

	void HitNearbyExplodables();

	bool setToIgnite = false;

private:
	float igniteTimer = 0.f;
};
