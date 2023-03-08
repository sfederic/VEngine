#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class InstanceMeshComponent;
struct InstanceData;

struct ProbeData
{
	XMFLOAT4 SH[9]{};
	XMFLOAT3 position = XMFLOAT3(0.f, 0.f, 0.f);
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

//There are a fuckload of references to do with GI, SH, Lightmapping and whatever else. Here are the best ones.
//Ref:https://therealmjp.github.io/posts/sg-series-part-1-a-brief-and-incomplete-history-of-baked-lighting-representations/
//Ref:https://therealmjp.github.io/posts/radiosity-dx11-style/
//Ref:https://morgan3d.github.io/articles/2019-04-01-ddgi/overview.html
//Ref:https://www.gdcvault.com/play/1428/Global-Illumination-in-SONIC
//Ref:https://chetanjags.wordpress.com/2015/08/26/image-based-lighting/
//Ref:https://www.flipcode.com/archives/Light_Mapping_Theory_and_Implementation.shtml
//Ref:https://interplayoflight.wordpress.com/2021/12/28/notes-on-occlusion-and-directionality-in-image-based-lighting/
//Ref:https://www.activision.com/cdn/research/Volumetric_Global_Illumination_at_Treyarch.pdf

//Even with all the above references, this implementation is actually based on a few slides (slide 49-52) from 
//Bluepoint's talk on global illumination in Demon's Souls, where they describe what they used for Shadow of the Collosus briefly.
//Ref:https://gdcvault.com/play/1027011/Advanced-Graphics-Summit-Lifting-the
class DiffuseProbeMap : public Actor
{
public:
	ACTOR_SYSTEM(DiffuseProbeMap);

	std::vector<ProbeData> probeData;

	InstanceMeshComponent* instanceMeshComponent = nullptr;

	int sizeX = 1;
	int sizeY = 1;
	int sizeZ = 1;

	DiffuseProbeMap();
	virtual void Create() override;
	virtual Properties GetProps() override;

	void SetProbeColour(XMFLOAT3 colour, uint32_t instanceMeshIndex);
	uint32_t GetProbeCount();
	ProbeData FindClosestProbe(XMVECTOR pos);
	void WriteProbeDataToFile();
	void SetProbeVisualColourFromIrradiance(ProbeData& pb, InstanceData& data);

private:
	void ReadProbeDataFromFile();
	void SetInstanceMeshData();
	std::string GetWorldNameAsFilename();
};
