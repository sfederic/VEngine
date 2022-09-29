#pragma once

#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Render/ShaderItem.h"

class Material;
struct Skeleton;

struct MeshComponent : SpatialComponent
{
	COMPONENT_SYSTEM(MeshComponent)

	//@Todo: need to figure out how to call this in World::Cleanup() because meshcomponent::create()
	//is called once on world load.
	static void ResetMeshBuffers();

	MeshComponentData meshComponentData;

	Material* material = nullptr;
	MeshDataProxy meshDataProxy;
	PipelineStateObject pso;

	//@Todo: think about splitting this up and currentAnimationTime into an AnimatedMeshComponent or something.
	Skeleton* GetSkeleton() { return meshDataProxy.skeleton; }
	std::string currentAnimation;
	std::string nextAnimation;
	float currentAnimationTime = 0.f;

	bool castsShadow = true;

	//whether the mesh is moved by physics system
	bool isStatic = true;

	bool skipPhysicsCreation = false;

	//whether nodes that spawn on this mesh are set as inactive
	bool gridObstacle = false;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		ShaderItem* shaderItem = ShaderItems::Default);
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetBlendState(const std::string newBlendState);
	void SetTexture(const std::string newTextureName);
	void SetShaderFilenames(ShaderItem& shaderItem);

	Buffer* GetVertexBuffer() const;
	Buffer* GetIndexBuffer() const;

	Material* GetMaterial() { return material; }
};
