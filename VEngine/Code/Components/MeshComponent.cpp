#include "vpch.h"
#include "MeshComponent.h"
#include "Asset/FBXLoader.h"
#include "Render/RenderUtils.h"
#include "Render/ShaderSystem.h"
#include "Render/Material.h"
#include "Render/MaterialSystem.h"
#include "Render/Renderer.h"
#include "Render/TextureSystem.h"
#include "Physics/PhysicsSystem.h"
#include "VMath.h"
#include "Camera.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"

void MeshComponent::ResetMeshBuffers()
{
	for (auto& meshBuffer : existingMeshBuffers)
	{
		meshBuffer.second.Destroy();
	}

	existingMeshBuffers.clear();
}

MeshComponent::MeshComponent()
{
	meshDataProxy = new MeshDataProxy();
	pso = new PipelineStateObject();

	//default values for material
	material = materialSystem.CreateMaterial("test.png", "DefaultShader.hlsl");
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

	//CullOnAngleBetweenCameraAndMesh();
	//SetBlendWhenBetweenPlayerAndCamera();
}

void MeshComponent::Create()
{
	//Set 'this' so mesh widgets can access this component.
	meshComponentData.meshComponent = this;

	//Material's create needs to be called here to deal with serialisation
	material->Create();

	//Import mesh (bounds extents set up in here too)
	FBXLoader::Import(meshComponentData.filename.c_str(), meshDataProxy);

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
	auto props = __super::GetProps();
	props.title = "MeshComponent";
	props.Add("Mesh", &meshComponentData).change = ReassignMesh;
	props.Add("Casts Shadow", &castsShadow);
	props.Add("Static", &isStatic);
	props.Add("Grid Obstacle", &gridObstacle);
	props.Add("Cull", &cull);
	props.Merge(material->GetProps());
	return props;
}

void MeshComponent::SetRastState(const std::string newRastStateName)
{
	//If calling before Material::Create() is called, rastStateName is the one.
	material->rastStateValue.SetValue(newRastStateName);
	//This set isn't necessary with the above, but fine for after Material::Create() called.
	material->rastState = Renderer::GetRastState(newRastStateName);
}

void MeshComponent::SetShaderName(const std::string newShaderName)
{
	material->shaderData.filename = newShaderName;
}

void MeshComponent::SetBlendState(const std::string newBlendState)
{
	material->blendState = Renderer::GetBlendState(newBlendState);
}

void MeshComponent::SetTexture(const std::string newTextureName)
{
	material->texture = textureSystem.FindTexture2D(newTextureName);
	material->textureData.filename = newTextureName;
}

Buffer* MeshComponent::GetVertexBuffer() const 
{
	return pso->vertexBuffer;
}

Buffer* MeshComponent::GetIndexBuffer() const
{
	return pso->indexBuffer;
}

void MeshComponent::CullOnAngleBetweenCameraAndMesh()
{
	if (cull)
	{
		const XMVECTOR meshForward = GetForwardVectorV();
		const XMVECTOR cameraPos = activeCamera->GetWorldPositionV();
		const XMVECTOR playerPos = GameUtils::GetPlayer()->GetPositionVector();
		const XMVECTOR cameraForward = playerPos - cameraPos;

		const float angle = XMConvertToDegrees(
			XMVector3AngleBetweenVectors(meshForward, cameraForward).m128_f32[0]);

		//The angles here are fairly specific to the player camera's position and lookat.
		//Use this function mainly for walls.
		cullMesh = angle > 110.f;
	}
}

void MeshComponent::SetBlendWhenBetweenPlayerAndCamera()
{
	const XMVECTOR meshPos = GetWorldPositionV();
	const XMVECTOR cameraPos = activeCamera->GetWorldPositionV();
	const XMVECTOR playerPos = GameUtils::GetPlayer()->GetPositionVector();

	const float cameraToMeshDistance = XMVector3Length(meshPos - cameraPos).m128_f32[0];
	const float cameraToPlayerDistance = XMVector3Length(playerPos - cameraPos).m128_f32[0];

	if (cameraToMeshDistance < cameraToPlayerDistance)
	{
		SetBlendState(BlendStates::Default);
	}
	else
	{
		SetBlendState(BlendStates::null);
	}
}
