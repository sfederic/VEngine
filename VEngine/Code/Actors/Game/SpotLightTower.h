#pragma once

#include "GridActor.h"

class SpotLightComponent;

class SpotLightTower : public GridActor
{
public:
	ACTOR_SYSTEM(SpotLightTower);

	SpotLightTower();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	void IlluminateGridActors();

	SpotLightComponent* spotLight = nullptr;
	float lightRange = 5.f;
};
