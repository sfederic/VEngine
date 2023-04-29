#pragma once

#include "GridActor.h"

class SpotLightComponent;

class SpotLightTower : public GridActor
{
public:
	ACTOR_SYSTEM(SpotLightTower);

	SpotLightTower();
	void Create() override;

private:
	SpotLightComponent* spotLight = nullptr;
};
