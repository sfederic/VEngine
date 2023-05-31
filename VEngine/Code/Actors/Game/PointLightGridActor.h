#pragma once

#include "GridActor.h"

class PointLightComponent;

class PointLightGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(PointLightGridActor);

	PointLightGridActor();

private:
	PointLightComponent* pointLight = nullptr;
};
