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

	//todo Setup physx bounds hull

	//Setup pipeline objects
	meshDataProxy = new MeshDataProxy();

	for (auto& meshData : meshDatas)
	{
		psos.push_back(PipelineStateObject());
		auto& pso = psos.back();

		pso.vertexBuffer = new Buffer();
		pso.indexBuffer = new Buffer();

		meshDataProxy->vertices = &meshData.vertices;
		meshDataProxy->indices = &meshData.indices;

		pso.vertexBuffer->data = RenderUtils::CreateVertexBuffer(meshDataProxy);
		pso.indexBuffer->data = RenderUtils::CreateIndexBuffer(meshDataProxy);
	}

	meshDataProxy->vertices = nullptr;
	meshDataProxy->indices = nullptr;
}

Properties DestructibleMeshComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "DestructibleMeshComponent";
    return props;
}
