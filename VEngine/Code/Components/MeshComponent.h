#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/ShaderItem.h"
#include "Render/VertexBuffer.h"
#include "Render/MeshDataProxy.h"

class Material;
class Skeleton;
class RastState;
class BlendState;

class MeshComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(MeshComponent);

	//Debug meshes are used by the renderer to display things like bounds, camera, lights, etc.
	static void CreateDebugMeshes();
	static void DestroyDebugMeshes();
	static MeshComponent* GetDebugMesh(std::string name);
	static std::vector<MeshComponent*> GetAllStaticMeshes();

	MeshComponentData meshComponentData;

	MeshDataProxy meshDataProxy;
	VertexBuffer vertexBuffer;

	//Cached indexes into the diffuse light probe map for static meshes, so that they're not constantly
	//searching out their closest light probe to take data from.
	int cachedLightProbeMapIndex = 0; //0 should be safe as a default, but it can cause problems.

	//This is for meshes that are messing with transparent meshes given that they're sorted by distance on render.
	//It's mostly used for floor and wall meshes where their position wouldn't line up with the space they're occupying.
	bool alwaysSortLast = false;

	bool castsShadow = true;

	bool skipPhysicsCreation = false;

	//Whether to make transparent when in between player and camera
	bool transparentOcclude = false;

	//For VagrantTactics. Tells the Grid that any nodes that are placed on this mesh are to become inactive.
	bool gridObstacle = false;
	bool ignoreGridRaycasts = false;
	bool canPlayerTraverse = true;

	bool canBeLinkedTo = true;

	MeshComponent();
	MeshComponent(const std::string filename_,
		const std::string textureFilename_,
		std::string shaderItemName = "Default");
	void Tick(float deltaTime) override;
	void Create() override;
	void Destroy() override;
	Properties GetProps() override;

	void ReCreate();

	void SplitMeshCreate();

	void SetMeshFilename(std::string_view meshFilename);
	auto GetMeshFilename() const { return meshComponentData.filename; }

	//Material set functions
	void SetRastState(const std::string newRastStateName);
	void SetBlendState(const std::string newBlendState);

	void SetTexture(const std::string newTextureName);
	std::string GetTextureFilename() const;

	void SetShaderItem(std::string shaderItemName);

	void SetAmbientColour(XMFLOAT3 ambientColour);
	XMFLOAT3 GetAmbientColour();
	void SetAlpha(float alpha);
	float GetAlpha();

	void SetUseTexture(bool useTexture);
	bool IsUsingTexture();

	void SetUVRotationSpeed(float speed);
	void SetUVOffsetSpeed(XMFLOAT2 speed);

	VertexBuffer& GetVertexBuffer();
	void CreateVertexBuffer();
	void CreateNewVertexBuffer();

	Material& GetMaterial() { return *material; }

	std::vector<Vertex>& GetAllVertices();
	std::vector<XMFLOAT3> GetAllVertexPositions();

	void SetCollisionMeshFilename(std::string_view filename) { collisionMeshFilename = filename; }
	std::string GetCollisionMeshFilename() { return collisionMeshFilename; }
	bool UsesCollisonMesh() { return !collisionMeshFilename.empty(); }

	BlendState& GetBlendState();
	RastState& GetRastState();

	bool IntersectsWithAnyBoundingBoxInWorld();

	void SetRenderStatic(bool renderStatic) { isRenderStatic = renderStatic; }
	bool IsRenderStatic() const { return isRenderStatic; }

	void SetPhysicsStatic(bool value) { isPhysicsStatic = value; }
	bool IsPhysicsStatic() const { return isPhysicsStatic; }

private:
	std::string collisionMeshFilename;

	Material* material = nullptr;

	bool isRenderStatic = true;

	//whether the mesh is moved by physics system
	bool isPhysicsStatic = true;
};
