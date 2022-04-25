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

//DiffuseProbeMap workflow:
//1. Place a DiffuseProbeMap in the world
//2. Set up the X, Y, Z extents to cover the world
//3. Use the 'BAKE' cmdline command 
//		Renderer will renderout cubemaps for every probe in the world and setup all spherical harmonic data.
//		SH data at this point is just a single ambient colour per probe. Meshes will take its closest light probe
//		and add its colour to its pixel output. Very simple stuff, mainly just for testing visuals.
//4. Reload the world 

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
