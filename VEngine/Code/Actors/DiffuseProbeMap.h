#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct ID3D11Texture3D;
struct ID3D11ShaderResourceView;

struct ProbeData
{
	float radiance;
};

struct DiffuseProbeMap : Actor
{
	ACTOR_SYSTEM(DiffuseProbeMap);

	ID3D11Texture3D* probeBuffer = nullptr;
	ID3D11ShaderResourceView* probeSRV = nullptr;

	int sizeX;
	int sizeY;
	int sizeZ;

	DiffuseProbeMap();
	virtual Properties GetProps() override;
};
