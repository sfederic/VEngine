#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct InstanceMeshComponent;
struct ID3D11Texture3D;
struct ID3D11ShaderResourceView;

struct ProbeData
{
	std::vector<XMFLOAT4> data;
};

//@Todo: Light probe maps aren't working great, if at all. Levels still look flat at this point with directional
//lighting so it'll help to integrate Spherical Harmonics into the engine properly.
struct DiffuseProbeMap : Actor
{
	ACTOR_SYSTEM(DiffuseProbeMap);

	ProbeData probeData;

	ID3D11Texture3D* probeMapTexture = nullptr;
	ID3D11ShaderResourceView* probeMapSRV = nullptr;

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
	XMFLOAT4 FindClosestProbe(XMVECTOR pos);

	void CreateProbeMapTexture();
};

