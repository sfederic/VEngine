#include "MeshComponent.h"
#include "FBXImporter.h"
#include "Render/RenderUtils.h"
#include "Render/ShaderSystem.h"
#include "Render/Materials/MaterialSystem.h"

std::unordered_map<std::string, PipelineStateObject*> existingPiplineStateObjects;

MeshComponent::MeshComponent(const char* filename_, const wchar_t* textureFilename)
{
	meshFilename = filename_;

	material = materialSystem.Find(textureFilename);

	data = new MeshDataProxy();
	pso = new PipelineStateObject();
}

void MeshComponent::Tick(double deltaTime)
{
}

void MeshComponent::Create()
{
	//Import mesh (set up bounding box in here too so you don't need to re-create bounds)
	fbxImporter.Import(meshFilename.c_str(), data);

	//Setup bounds
	BoundingOrientedBox::CreateFromPoints(boundingBox, data->vertices->size(),
		&data->vertices->at(0).pos, sizeof(Vertex));

	//Setup pipeline objects
	auto psoIt = existingPiplineStateObjects.find(meshFilename);
	if (psoIt == existingPiplineStateObjects.end())
	{
		pso->vertexBuffer.data = RenderUtils::CreateVertexBuffer(data);
		pso->indexBuffer.data = RenderUtils::CreateIndexBuffer(data);
	}
	else
	{
		pso->vertexBuffer = psoIt->second->vertexBuffer;
		pso->indexBuffer = psoIt->second->indexBuffer;
	}

	existingPiplineStateObjects[meshFilename] = pso;
}

Properties MeshComponent::GetProps()
{
	Properties props("MeshComponent");
	props.Add("Mesh Filename", &meshFilename);
	return props;
}
