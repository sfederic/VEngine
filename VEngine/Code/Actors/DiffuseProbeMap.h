#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct ProbeData
{
	float radiance;
};

struct DiffuseProbeMap : Actor
{
	ACTOR_SYSTEM(DiffuseProbeMap);

	int sizeX;
	int sizeY;
	int sizeZ;

	DiffuseProbeMap();
	virtual Properties GetProps() override;
};
