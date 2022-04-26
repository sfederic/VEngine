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
//		and add its colour to its pixel output.
//4. Reload the world 

//@Todo: The diffuse GI here is mainly just for visual testing, just to get an idea on how it would look with low-poly.
//To make it work properly, the light probe data needs to be serialised out. It honestly looks pretty shit against
//low-poly environments, a better approach might be light mapping.

//There are a fuckload of references to do with GI, SH, Lightmapping and whatever else. Here are the best ones.
//Ref:https://therealmjp.github.io/posts/sg-series-part-1-a-brief-and-incomplete-history-of-baked-lighting-representations/
//Ref:https://therealmjp.github.io/posts/radiosity-dx11-style/
//Ref:https://morgan3d.github.io/articles/2019-04-01-ddgi/overview.html
//Ref:https://www.gdcvault.com/play/1428/Global-Illumination-in-SONIC
//Ref:https://chetanjags.wordpress.com/2015/08/26/image-based-lighting/
//Ref:https://www.flipcode.com/archives/Light_Mapping_Theory_and_Implementation.shtml
//Ref:https://interplayoflight.wordpress.com/2021/12/28/notes-on-occlusion-and-directionality-in-image-based-lighting/
//Ref:https://www.activision.com/cdn/research/Volumetric_Global_Illumination_at_Treyarch.pdf

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
