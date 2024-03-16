#pragma once

#include "Actor.h"
#include "ActorSystem.h"
#include "Render/ShaderData/InstanceData.h"
#include "Actors/DebugActors/DebugLightProbe.h"
#include <wrl.h>

class InstanceMeshComponent;
struct InstanceData;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//DiffuseProbeMap workflow:
//1. Place a DiffuseProbeMap in the world
//2. Set up the X, Y, Z extents to cover the world
//3. Use the 'BAKE' cmdline command 
//		Renderer will render out cubemaps for every probe in the world and setup all spherical harmonic data.
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
//Bluepoint's talk on global illumination in Demon's Souls, where they describe what they used for Shadow of the Colossus briefly.
//Ref:https://gdcvault.com/play/1027011/Advanced-Graphics-Summit-Lifting-the
class DiffuseProbeMap : public Actor
{
public:
	ACTOR_SYSTEM(DiffuseProbeMap);

	DiffuseProbeMap();
	void Create() override;
	void PostCreate() override;
	Properties GetProps() override;

	uint32_t GetProbeCount();
	LightProbeInstanceData GetProbeByIndex(int index);
	LightProbeInstanceData FindClosestProbe(XMVECTOR pos);
	void WriteProbeDataToFile();

	ID3D11Buffer* GetStructuredBuffer() { return structuredBuffer.Get(); }
	ID3D11ShaderResourceView* GetSRV() { return srv.Get(); }

	auto GetInstanceMesh() { return lightProbesDebugInstanceMesh->instanceMesh; }
	auto& GetLightProbeData() { return lightProbeData; }

private:
	void ReadProbeDataFromFile();
	void SetLightProbeData();
	std::string GetWorldNameAsFilename();

	void AssignStaticMeshesLightProbeIndex();

	Microsoft::WRL::ComPtr<ID3D11Buffer> structuredBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	DebugLightProbe* lightProbesDebugInstanceMesh = nullptr;

	std::vector<LightProbeInstanceData> lightProbeData;

	int sizeX = 1;
	int sizeY = 1;
	int sizeZ = 1;
};
