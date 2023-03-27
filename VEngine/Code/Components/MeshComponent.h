#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/PipelineObjects.h"
#include "Render/ShaderItem.h"
#include "Render/MeshDataProxy.h"

class Material;
struct Skeleton;

class MeshComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(MeshComponent)

	static void ResetMeshBuffers();

	//Debug meshes are used by the renderer to display things like bounds, camera, lights, etc.
	static void CreateDebugMeshes();
	static void DestroyDebugMeshes();
	static MeshComponent* GetDebugMesh(std::string name);

	static std::vector<MeshComponent*> SortMeshComponentsByDistance();

	MeshComponentData meshComponentData;

	MeshDataProxy meshDataProxy;
	PipelineStateObject pso;

	bool castsShadow = true;

	//whether the mesh is moved by physics system
	bool isStatic = true;

	bool skipPhysicsCreation = false;

	//Whether to make transparent when in between player and camera
	bool transparentOcclude = false;

	bool gridObstacle = false;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		ShaderItem* shaderItem = ShaderItems::Default);
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();

	void SplitMeshCreate();

	void SetMeshFilename(std::string_view meshFilename);

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetBlendState(const std::string newBlendState);

	void SetTexture(const std::string newTextureName);
	std::string GetTextureFilename();

	void SetShaderFilenames(ShaderItem* shaderItem);

	void SetAmbientColour(const XMFLOAT4 ambientColour);
	XMFLOAT4 GetAmbientColour();

	void SetUseTexture(bool useTexture);
	bool IsUsingTexture();

	void SetUVRotationSpeed(float speed);
	void SetUVOffsetSpeed(XMFLOAT2 speed);

	Buffer* GetVertexBuffer() const;
	void CreateVertexBuffer();

	Material& GetMaterial() { return *material; }

	std::vector<XMFLOAT3> GetAllVertexPositions();

	void SetCollisionMeshFilename(std::string_view filename) { collisionMeshFilename = filename; }
	std::string GetCollisionMeshFilename() { return collisionMeshFilename; }
	bool UsesCollisonMesh() { return !collisionMeshFilename.empty(); }

private:
	std::string collisionMeshFilename;

	Material* material = nullptr;
};
