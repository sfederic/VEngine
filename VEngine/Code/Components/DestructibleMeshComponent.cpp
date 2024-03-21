#include "vpch.h"
#include "DestructibleMeshComponent.h"
#include "Asset/FBXLoader.h"
#include "Render/Material.h"

DestructibleMeshComponent::DestructibleMeshComponent(const std::string filename_,
	const std::string textureFilename_,
	const std::string shaderItemName)
	: MeshComponent(filename_, textureFilename_, shaderItemName)
{
}

void DestructibleMeshComponent::Create()
{
	meshComponentData.meshComponent = this;

	GetMaterial().Create();

	FBXLoader::ImportFracturedMesh(meshComponentData.filename.c_str(), meshDatas);

	int meshIndex = 0;

	for (auto& meshData : meshDatas)
	{
		//@Todo: because the mesh components here have no actor parent, their deserialisation
		//on world load will crash over owner UIDs not being found.
		auto mesh = MeshComponent::system.Add("TempDestructibleMesh" + std::to_string(meshIndex));

		//parent all the fractured cell meshes to this to be able to move it around before breaking.
		this->AddChild(mesh);

		mesh->isPhysicsStatic = false;

		mesh->meshDataProxy.boundingBox = &meshData.boundingBox;
		mesh->meshDataProxy.vertices = meshData.vertices;
		mesh->meshDataProxy.skeleton = &meshData.skeleton;

		//Setup bounds
		auto meshBoundingBox = mesh->GetBoundsInWorldSpace();
		BoundingOrientedBox::CreateFromPoints(meshBoundingBox, mesh->meshDataProxy.vertices.size(),
			&mesh->meshDataProxy.vertices.at(0).pos, sizeof(Vertex));

		mesh->CreateVertexBuffer();

		mesh->meshComponentData.filename = "destructible_cube.vmesh";

		meshComponentData.meshComponent = this;

		mesh->GetMaterial().Create();

		meshCells.emplace_back(mesh);

		meshIndex++;
	}
}

Properties DestructibleMeshComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
