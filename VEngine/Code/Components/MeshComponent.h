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

	//For VagrantTactics. Tells the Grid that any nodes that are placed on this mesh are to become inactive.
	bool gridObstacle = false;
	bool ignoreGridRaycasts = false;

	bool canBeLinkedTo = true;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		ShaderItem* shaderItem = ShaderItems::Default);
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps();

	void ReCreate();

	void SplitMeshCreate();

	void SetMeshFilename(std::string_view meshFilename);
	auto GetMeshFilename() { return meshComponentData.filename; }

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetBlendState(const std::string newBlendState);

	void SetTexture(const std::string newTextureName);
	std::string GetTextureFilename();

	void SetShaderItem(ShaderItem* shaderItem);

	void SetAmbientColour(const XMFLOAT4 ambientColour);
	XMFLOAT4 GetAmbientColour();
	void SetAlpha(float alpha);

	void SetUseTexture(bool useTexture);
	bool IsUsingTexture();

	void SetUVRotationSpeed(float speed);
	void SetUVOffsetSpeed(XMFLOAT2 speed);

	Buffer& GetVertexBuffer();
	void CreateVertexBuffer();
	void CreateNewVertexBuffer();

	Material& GetMaterial() { return *material; }

	std::vector<XMFLOAT3> GetAllVertexPositions();

	void SetCollisionMeshFilename(std::string_view filename) { collisionMeshFilename = filename; }
	std::string GetCollisionMeshFilename() { return collisionMeshFilename; }
	bool UsesCollisonMesh() { return !collisionMeshFilename.empty(); }

	BlendState& GetBlendState();
	RastState& GetRastState();

private:
	std::string collisionMeshFilename;

	Material* material = nullptr;
};
