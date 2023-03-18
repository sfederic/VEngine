#include "vpch.h"
#include "DestructibleMeshComponent.h"
#include "Asset/FBXLoader.h"
#include "Render/Material.h"
#include "Render/RenderUtils.h"

DestructibleMeshComponent::DestructibleMeshComponent(const std::string filename_,
    const std::string textureFilename_,
    ShaderItem* shaderItem)
    : MeshComponent(filename_, textureFilename_, shaderItem)
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

		meshCells.emplace_back(mesh);

		mesh->isStatic = false;

		mesh->meshDataProxy.vertices = &meshData.vertices;

		//Setup bounds
		auto meshBoundingBox = mesh->GetBoundingBox();
		BoundingOrientedBox::CreateFromPoints(meshBoundingBox, mesh->meshDataProxy.vertices->size(),
			&mesh->meshDataProxy.vertices->at(0).pos, sizeof(Vertex));

		mesh->pso.vertexBuffer = new Buffer();
		mesh->pso.vertexBuffer->data = RenderUtils::CreateVertexBuffer(mesh->meshDataProxy);

		meshIndex++;
	}
}

Properties DestructibleMeshComponent::GetProps()
{
    auto props = __super::GetProps();
	props.title = GetTypeName();
    return props;
}
