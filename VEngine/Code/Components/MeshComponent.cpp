#include "vpch.h"
#include "MeshComponent.h"
#include <algorithm>
#include "Core/Camera.h"
#include "Asset/AssetSystem.h"
#include "Render/Material.h"
#include "Render/MaterialSystem.h"
#include "Render/Renderer.h"
#include "Render/TextureSystem.h"
#include "Physics/PhysicsSystem.h"

std::unordered_map<std::string, MeshComponent*> debugMeshes;

void MeshComponent::CreateDebugMeshes()
{
	debugMeshes.emplace("DebugBox", new MeshComponent("cube.vmesh", "test.png"));
	debugMeshes.emplace("DebugCamera", new MeshComponent("camera.vmesh", "test.png"));
	debugMeshes.emplace("DebugCapsule", new MeshComponent("capsule.vmesh", "test.png"));
	debugMeshes.emplace("DebugCone", new MeshComponent("small_cone.vmesh", "test.png"));
	debugMeshes.emplace("DebugIcoSphere", new MeshComponent("small_ico_sphere.vmesh", "test.png"));
	debugMeshes.emplace("DebugSphere", new MeshComponent("ico_sphere.vmesh", "test.png"));

	for (auto& [name, mesh] : debugMeshes)
	{
		mesh->Create();
	}
}

void MeshComponent::DestroyDebugMeshes()
{
	for (auto& [name, mesh] : debugMeshes)
	{
		mesh->Destroy();
		delete mesh;
	}

	debugMeshes.clear();
}

MeshComponent* MeshComponent::GetDebugMesh(std::string name)
{
	return debugMeshes.find(name)->second;
}

std::vector<MeshComponent*> MeshComponent::GetAllStaticMeshes()
{
	std::vector<MeshComponent*> meshes;
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		if (mesh->IsRenderStatic())
		{
			meshes.push_back(mesh.get());
		}
	}
	return meshes;
}

//@Todo: this is still not a great solution for transparency. Lot of meshes are overlapping their transparency
//based on distance. Look into some helpful add-on solution, maybe even something dirty like:
//"If multiple meshes have same world position, bring transparent blend state actors to front."
std::vector<MeshComponent*> MeshComponent::SortMeshComponentsByDistance()
{
	struct MeshPack
	{
		MeshComponent* mesh = nullptr;
		float distance = 0.f;
	};
	std::vector<MeshPack> meshPacks;

	const XMVECTOR cameraPos = Camera::GetActiveCamera().GetWorldPositionV();

	for (auto& mesh : system.GetComponents())
	{
		float distance = XMVector3Length(cameraPos - mesh->GetWorldPositionV()).m128_f32[0];
		if (mesh->alwaysSortLast)
		{
			distance = std::numeric_limits<float>::max();
		}
		MeshPack pack = { mesh.get(), distance };
		meshPacks.emplace_back(pack);
	}

	auto DistCompare = [](const MeshPack& leftPack, const MeshPack& rightPack)
		{
			return leftPack.distance > rightPack.distance;
		};
	std::sort(meshPacks.begin(), meshPacks.end(), DistCompare);

	std::vector<MeshComponent*> sortedMeshes;
	for (auto& pack : meshPacks)
	{
		sortedMeshes.emplace_back(pack.mesh);
	}
	return sortedMeshes;
}

MeshComponent::MeshComponent()
{
	material = &MaterialSystem::CreateMaterial("test.png", "Default");
}

MeshComponent::MeshComponent(const std::string filename_,
	const std::string textureFilename_,
	std::string shaderItemName)
{
	meshComponentData.filename = filename_;
	material = &MaterialSystem::CreateMaterial(textureFilename_, shaderItemName);
}

void MeshComponent::Tick(float deltaTime)
{
	if (!isPhysicsStatic)
	{
		PhysicsSystem::GetTransformFromPhysicsActor(this);
	}
}

void MeshComponent::Create()
{
	//Set 'this' so mesh widgets can access this component.
	meshComponentData.meshComponent = this;

	//Material's create needs to be called here to deal with serialisation
	material->Create();

	meshDataProxy = AssetSystem::ReadVMeshAssetFromFile(meshComponentData.filename);

	BoundingOrientedBox::CreateFromBoundingBox(boundingBox, *meshDataProxy.boundingBox);

	CreateVertexBuffer();
}

void MeshComponent::Destroy()
{
	//Erase physics actor
	PhysicsSystem::ReleasePhysicsActor(this);

	material->Destroy();
	material = nullptr;

	vertexBuffer.Destroy();
}

static void ReassignMesh(void* data)
{
	auto meshData = (MeshComponentData*)data;
	meshData->meshComponent->ReCreate();
}

Properties MeshComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();

	props.Add("Mesh", &meshComponentData).change = ReassignMesh;
	props.Add("Casts Shadow", &castsShadow);
	props.Add("Physics Static", &isPhysicsStatic);
	props.Add("Render Static", &isRenderStatic);
	props.Add("Trans. Occlude", &transparentOcclude);
	props.Add("Grid Obst.", &gridObstacle);
	props.Add("Grid Ignore", &ignoreGridRaycasts);
	props.Add("Player Traversable", &canPlayerTraverse);
	props.Add("Sort Last", &alwaysSortLast);

	auto materialProps = material->GetProps();
	materialProps.SetAllPropertyOwnerUIDs(props.ownerUID);
	props.Merge(materialProps);

	return props;
}

void MeshComponent::ReCreate()
{
	material->Create();
	meshDataProxy = AssetSystem::ReadVMeshAssetFromFile(meshComponentData.filename);
	CreateNewVertexBuffer();
}

void MeshComponent::SplitMeshCreate()
{
	meshComponentData.meshComponent = this;

	material->Create();

	vertexBuffer.CreateDefault(meshDataProxy);

	//Make sure bounds setup is before physics actor creation
	BoundingBox bb;
	BoundingBox::CreateFromPoints(bb, meshDataProxy.vertices.size(),
		&meshDataProxy.vertices.at(0).pos, sizeof(Vertex));

	//@Todo: owner being set as null on CreatePhysicsActor() won't explode the program,
	//but it will cause problems if you want to use raycasts via PhysX.
	if (meshDataProxy.vertices.size() > 255)
	{
		PhysicsSystem::CreatePhysicsActor(this, PhysicsType::Dynamic, GetOwner());
	}
	else
	{
		PhysicsSystem::CreateConvexPhysicsMesh(this, GetOwner());
	}
}

void MeshComponent::SetMeshFilename(std::string_view meshFilename)
{
	meshComponentData.filename = meshFilename;
}

void MeshComponent::SetRastState(const std::string newRastStateName)
{
	//If calling before Material::Create() is called, rastStateName is the one.
	material->rastStateValue.SetValue(newRastStateName);
	//This set isn't necessary with the above, but fine for after Material::Create() called.
	material->rastState = Renderer::GetRastState(newRastStateName);
}

void MeshComponent::SetBlendState(const std::string newBlendState)
{
	material->blendStateValue.SetValue(newBlendState);
	material->blendState = Renderer::GetBlendState(newBlendState);
}

void MeshComponent::SetTexture(const std::string newTextureName)
{
	material->defaultTexture = TextureSystem::FindTexture2D(newTextureName);
	material->defaultTextureData.filename = newTextureName;
}

std::string MeshComponent::GetTextureFilename()
{
	return material->defaultTextureData.filename;
}

void MeshComponent::SetShaderItem(std::string shaderItemName)
{
	material->shaderItemValue.SetValue(shaderItemName);
}

void MeshComponent::SetAmbientColour(XMFLOAT3 ambientColour)
{
	material->materialShaderData.ambient.x = ambientColour.x;
	material->materialShaderData.ambient.y = ambientColour.y;
	material->materialShaderData.ambient.z = ambientColour.z;
}

XMFLOAT3 MeshComponent::GetAmbientColour()
{
	XMFLOAT3 colour = {};
	colour.x = material->materialShaderData.ambient.x;
	colour.y = material->materialShaderData.ambient.y;
	colour.z = material->materialShaderData.ambient.z;
	return colour;
}

void MeshComponent::SetAlpha(float alpha)
{
	material->materialShaderData.ambient.w = alpha;
}

float MeshComponent::GetAlpha()
{
	return material->materialShaderData.ambient.w;
}

void MeshComponent::SetUseTexture(bool useTexture)
{
	material->materialShaderData.useTexture = useTexture;
}

bool MeshComponent::IsUsingTexture()
{
	return material->materialShaderData.useTexture;
}

void MeshComponent::SetUVRotationSpeed(float speed)
{
	material->uvRotationSpeed = speed;
}

void MeshComponent::SetUVOffsetSpeed(XMFLOAT2 speed)
{
	material->uvOffsetSpeed = speed;
}

VertexBuffer& MeshComponent::GetVertexBuffer()
{
	return vertexBuffer;
}

void MeshComponent::CreateVertexBuffer()
{
	vertexBuffer.CreateDefault(meshDataProxy);
}

void MeshComponent::CreateNewVertexBuffer()
{
	vertexBuffer.Destroy();
	vertexBuffer.CreateDefault(meshDataProxy);
}

std::vector<Vertex>& MeshComponent::GetAllVertices()
{
	return meshDataProxy.vertices;
}

std::vector<XMFLOAT3> MeshComponent::GetAllVertexPositions()
{
	std::vector<XMFLOAT3> positions;
	for (auto& vertex : meshDataProxy.vertices)
	{
		positions.emplace_back(vertex.pos);
	}
	return positions;
}

BlendState& MeshComponent::GetBlendState()
{
	return *material->blendState;
}

RastState& MeshComponent::GetRastState()
{
	return *material->rastState;
}

bool MeshComponent::IntersectsWithAnyBoundingBoxInWorld()
{
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		const auto thisMeshBounds = GetBoundsInWorldSpace();
		if (thisMeshBounds.Intersects(mesh->GetBoundsInWorldSpace()))
		{
			return true;
		}
	}

	return false;
}
