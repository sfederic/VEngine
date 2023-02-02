#pragma once

#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Render/ShaderItem.h"

class Material;
struct Skeleton;

class MeshComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(MeshComponent)

	//@Todo: need to figure out how to call this in World::Cleanup() because meshcomponent::create()
	//is called once on world load.
	static void ResetMeshBuffers();

	static std::vector<MeshComponent*> SortMeshComponentsByDistanceToCamera();

	MeshComponentData meshComponentData;

	Material* material = nullptr;
	MeshDataProxy meshDataProxy;
	PipelineStateObject pso;

	bool castsShadow = true;

	//whether the mesh is moved by physics system
	bool isStatic = true;

	bool skipPhysicsCreation = false;

	//Whether to make transparent when in between player and camera
	bool transparentOcclude = false;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		ShaderItem* shaderItem = ShaderItems::Default);
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();

	void SplitMeshCreate();

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetBlendState(const std::string newBlendState);
	void SetTexture(const std::string newTextureName);
	void SetShaderFilenames(ShaderItem* shaderItem);

	void SetAmbientColour(const XMFLOAT4 ambientColour);
	XMFLOAT4 GetAmbientColour();

	void SetUseTexture(bool useTexture);

	Buffer* GetVertexBuffer() const;

	Material* GetMaterial() { return material; }

	std::vector<XMFLOAT3> GetAllVertexPositions();
};
