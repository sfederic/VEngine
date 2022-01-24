#include "MeshComponent.h"
#include "Asset/FBXLoader.h"
#include "Render/RenderUtils.h"
#include "Render/ShaderSystem.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "WorldEditor.h"
#include "Editor/Editor.h"
#include "Animation/AnimationStructures.h"
#include "Physics/PhysicsSystem.h"
#include "VMath.h"

void MeshComponent::ResetMeshBuffers()
{
	for (auto& meshIt : existingMeshBuffers)
	{
		MeshBuffers& buffers = meshIt.second;

		buffers.vertexBuffer->data->Release();
		delete buffers.vertexBuffer;

		buffers.indexBuffer->data->Release();
		delete buffers.indexBuffer;
	}

	existingMeshBuffers.clear();
}

MeshComponent::MeshComponent()
{
	meshDataProxy = new MeshDataProxy();
	pso = new PipelineStateObject();

	material = new Material("test.png", "DefaultShader.hlsl");
}

MeshComponent::MeshComponent(const std::string filename_,
	const std::string textureFilename_,
	const std::string shaderFilename_)
{
	meshDataProxy = new MeshDataProxy();
	pso = new PipelineStateObject();

	meshComponentData.filename = filename_;

	material = new Material(textureFilename_, shaderFilename_);
}

void MeshComponent::Start()
{

}

void MeshComponent::Tick(float deltaTime)
{
	if (!isStatic)
	{
		physicsSystem.GetTransformFromPhysicsActor(this);
	}
}

void MeshComponent::Create()
{
	//Set 'this' so mesh widgets can access this component.
	meshComponentData.meshComponent = this;

	//Material's create needs to be called here to deal with serialisation
	material->Create();

	//Import mesh (bounds extents set up in here too)
	fbxLoader.Import(meshComponentData.filename.c_str(), meshDataProxy);

	//Setup bounds
	BoundingOrientedBox::CreateFromBoundingBox(boundingBox, *meshDataProxy->boundingBox);

	//Setup pipeline objects
	auto psoIt = existingMeshBuffers.find(meshComponentData.filename);
	if (psoIt == existingMeshBuffers.end())
	{
		pso->vertexBuffer = new Buffer();
		pso->indexBuffer = new Buffer();
		pso->vertexBuffer->data = RenderUtils::CreateVertexBuffer(meshDataProxy);
		pso->indexBuffer->data = RenderUtils::CreateIndexBuffer(meshDataProxy);
	}
	else
	{
		pso->vertexBuffer = psoIt->second.vertexBuffer;
		pso->indexBuffer = psoIt->second.indexBuffer;
	}

	pso->Create();

	MeshBuffers meshBuffers = {};
	meshBuffers.vertexBuffer = pso->vertexBuffer;
	meshBuffers.indexBuffer = pso->indexBuffer;
	existingMeshBuffers[meshComponentData.filename] = meshBuffers;
}

void MeshComponent::Destroy()
{
	//Erase physics actor
	physicsSystem.ReleasePhysicsActor(this);

	material->Destroy();
	delete material;
	material = nullptr;

	delete meshDataProxy;
	meshDataProxy = nullptr;

	delete pso;
	pso = nullptr;
}

static void ReassignMesh(void* data)
{
	auto meshData = (MeshComponentData*)data;
	meshData->meshComponent->Create();
}

Properties MeshComponent::GetProps()
{
	Properties props("MeshComponent");
	props.Add("Mesh", &meshComponentData).change = ReassignMesh;
	props.Add("Casts Shadow", &castsShadow);
	props.Add("Static", &isStatic);
	props.Add("Grid Obstacle", &gridObstacle);
	props.Merge(material->GetProps());
	return props;
}

void MeshComponent::SetRastState(const std::string newRastStateName)
{
	material->rastState = renderer.rastStateMap[newRastStateName];
}
