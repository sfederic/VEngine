#include "MeshComponent.h"
#include "FBXImporter.h"
#include "Render/RenderUtils.h"
#include "Render/ShaderSystem.h"
#include "Render/Material.h"

//Vertex and index buffers linked to a mesh filename to copy over to new PSOs
std::unordered_map<std::string, MeshBuffers> existingMeshBuffers;

MeshComponent::MeshComponent(const char* filename_, const wchar_t* textureFilename_)
{
	meshFilename = filename_;
	textureFilename = textureFilename_;
}

void MeshComponent::Tick(double deltaTime)
{
}

void MeshComponent::Create()
{
	data = new MeshDataProxy();
	pso = new PipelineStateObject();
	material = new Material(textureFilename, L"DefaultShader.hlsl");

	//Import mesh (set up bounding box in here too so you don't need to re-create bounds)
	fbxImporter.Import(meshFilename.c_str(), data);

	//Setup bounds
	BoundingOrientedBox::CreateFromPoints(boundingBox, data->vertices->size(),
		&data->vertices->at(0).pos, sizeof(Vertex));

	//Setup pipeline objects
	auto psoIt = existingMeshBuffers.find(meshFilename);
	if (psoIt == existingMeshBuffers.end())
	{
		pso->vertexBuffer = new Buffer();
		pso->indexBuffer = new Buffer();
		pso->vertexBuffer->data = RenderUtils::CreateVertexBuffer(data);
		pso->indexBuffer->data = RenderUtils::CreateIndexBuffer(data);
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
	existingMeshBuffers[meshFilename] = meshBuffers;
}

Properties MeshComponent::GetProps()
{
	Properties props("MeshComponent");
	props.Add("Mesh Filename", &meshFilename);
	props.Add("UV offset", &material->shaderData.uvOffset);
	return props;
}
