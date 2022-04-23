#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct InstanceMeshComponent;

struct ProbeData
{
	float radiance;
};

struct DiffuseProbeMap : Actor
{
	ACTOR_SYSTEM(DiffuseProbeMap);

	InstanceMeshComponent* instanceMeshComponent = nullptr;

	int sizeX = 1;
	int sizeY = 1;
	int sizeZ = 1;

	DiffuseProbeMap();
	virtual Properties GetProps() override;
	void SetInstanceMeshData();
};
