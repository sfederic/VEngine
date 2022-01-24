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

	//Vertex and index buffers linked to a mesh filename to copy over to new PSOs
	static inline std::unordered_map<std::string, MeshBuffers> existingMeshBuffers;
	//@Todo: need to figure out how to call this in World::Cleanup() because meshcomponent::create()
	//is called once on world load.
	static void ResetMeshBuffers();

	MeshComponentData meshComponentData;

	Material* material = nullptr;
	MeshDataProxy* meshDataProxy = nullptr;
	PipelineStateObject* pso = nullptr;

	//@Todo: think about splitting this up and currentAnimationTime into an AnimatedMeshComponent or something.
	Skeleton* GetSkeleton() { return meshDataProxy->skeleton; }
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
		const std::string shaderFilename_ = "DefaultShader.hlsl");
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetShaderName(const std::string newShaderName);
};
