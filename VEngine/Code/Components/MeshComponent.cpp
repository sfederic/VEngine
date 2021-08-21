#include "MeshComponent.h"
#include "FBXImporter.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"

std::unordered_map<std::string, PipelineStateObject*> existingPiplineStateObjects;

MeshComponent::MeshComponent(const char* filename_, const wchar_t* shaderFilename)
{
	filename = filename_;
	data = new MeshDataProxy();
	pso = new PipelineStateObject();

	shader = shaderSystem.shaderMap.find(shaderFilename)->second;
}

void MeshComponent::Create()
{
	//Import mesh (set up bounding box in here too so you don't need to re-create bounds)
	fbxImporter.Import(filename.c_str(), data);

	//Setup bounds
	BoundingOrientedBox::CreateFromPoints(boundingBox, data->vertices->size(),
		&data->vertices->at(0).pos, sizeof(Vertex));

	//Setup pipeline objects
	auto psoIt = existingPiplineStateObjects.find(filename);
	if (psoIt == existingPiplineStateObjects.end())
	{
		pso->vertexBuffer.data = renderer.CreateVertexBuffer(data);
		pso->indexBuffer.data = renderer.CreateIndexBuffer(data);
		pso->sampler.data = renderer.CreateSampler();
	}
	else
	{
		pso = psoIt->second;
	}

	existingPiplineStateObjects[filename] = pso;
}

Properties MeshComponent::GetProperties()
{
	return Properties();
}
