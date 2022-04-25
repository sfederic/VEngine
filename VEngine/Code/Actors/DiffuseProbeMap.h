#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct InstanceMeshComponent;

struct ProbeData
{
	XMFLOAT4 SH[9];
	XMFLOAT3 position;
	int index = 0;
};

//@Todo: Light probe maps aren't working great, if at all. Levels still look flat at this point with directional
//lighting so it'll help to integrate Spherical Harmonics into the engine properly.
struct DiffuseProbeMap : Actor
{
	ACTOR_SYSTEM(DiffuseProbeMap);

	std::vector<ProbeData> probeData;

	InstanceMeshComponent* instanceMeshComponent = nullptr;

	int sizeX = 1;
	int sizeY = 1;
	int sizeZ = 1;

	DiffuseProbeMap();
	virtual void Create() override;
	virtual Properties GetProps() override;
	void SetInstanceMeshData();
	void SetProbeColour(XMFLOAT3 colour, uint32_t instanceMeshIndex);
	uint32_t GetProbeCount();
	ProbeData FindClosestProbe(XMVECTOR pos);
};
