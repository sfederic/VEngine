#include "DestructibleMeshComponent.h"
#include "Asset/FBXLoader.h"
#include "Render/Material.h"
#include "Render/RenderUtils.h"

DestructibleMeshComponent::DestructibleMeshComponent(const std::string filename_,
    const std::string textureFilename_,
    const std::string shaderFilename_)
    : MeshComponent(filename_, textureFilename_, shaderFilename_)
{
}

void DestructibleMeshComponent::Create()
{
	meshComponentData.meshComponent = this;

	material->Create();

	fbxLoader.ImportFracturedMesh(meshComponentData.filename.c_str(), meshDatas);

	for (auto& meshData : meshDatas)
	{
		auto mesh = MeshComponent::system.Add();

		//parent all the fractured cell meshes to this to be able to move it around before breaking.
		this->AddChild(mesh);

		meshCells.push_back(mesh);

		mesh->isStatic = false;

		mesh->meshDataProxy->vertices = &meshData.vertices;
		mesh->meshDataProxy->indices = &meshData.indices;

		//Setup bounds
		BoundingOrientedBox::CreateFromPoints(mesh->boundingBox, mesh->meshDataProxy->vertices->size(),
			&mesh->meshDataProxy->vertices->at(0).pos, sizeof(Vertex));

		mesh->pso->vertexBuffer = new Buffer();
		mesh->pso->indexBuffer = new Buffer();
		mesh->pso->vertexBuffer->data = RenderUtils::CreateVertexBuffer(mesh->meshDataProxy);
		mesh->pso->indexBuffer->data = RenderUtils::CreateIndexBuffer(mesh->meshDataProxy);
	}
}

Properties DestructibleMeshComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "DestructibleMeshComponent";
    return props;
}
