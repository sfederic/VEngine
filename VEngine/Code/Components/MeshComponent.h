#pragma once
#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Render/ShaderPair.h"

class Material;
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
	std::string currentAnimation;
	float currentAnimationTime = 0.f;

	bool castsShadow = true;

	//whether the mesh is moved by physics system
	bool isStatic = true;

	bool skipPhysicsCreation = false;

	//whether nodes that spawn on this mesh are set as inactive
	bool gridObstacle = false;

private:
	//Only set for meshes that obstruct view between player and camera
	bool cull = false;

public:
	bool cullMesh = false;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		ShaderPairNames shaderPair = ShaderPairs::Default);
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetBlendState(const std::string newBlendState);
	void SetTexture(const std::string newTextureName);
	void SetShaderPair(ShaderPairNames shaderPair);

	Buffer* GetVertexBuffer() const;
	Buffer* GetIndexBuffer() const;

private:
	void CullOnAngleBetweenCameraAndMesh();
	void SetBlendWhenBetweenPlayerAndCamera();
};
