#pragma once
#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Transform.h"

struct ShaderItem;
struct Material;
struct Skeleton;

struct MeshComponent : SpatialComponent
{
	COMPONENT_SYSTEM(MeshComponent)

	MeshComponentData meshComponentData;

	Material* material = nullptr;
	MeshDataProxy* meshDataProxy = nullptr;
	PipelineStateObject* pso = nullptr;

	//@Todo: think about splitting this up into an AnimatedMeshComponent or something.
	//The reason it's here is because you need all the joint transforms and timing per mesh to be unique.
	Skeleton* skeleton = nullptr;

	bool castsShadow = true;

	//whether nodes that spawn on this mesh are set as inactive
	bool gridObstacle = false;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		const std::string shaderFilename_ = "DefaultShader.hlsl");
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();
};
