#include "MeshComponent.h"
#include "FBXImporter.h"
#include "Render/Renderer.h"
#include "Render/ShaderSystem.h"

std::unordered_map<std::string, PipelineStateObject*> existingPiplineStateObjects;

MeshComponent::MeshComponent(const char* filename_, std::wstring textureName, const wchar_t* shaderFilename)
{
	filename = filename_;
	textureFilename = textureName;

	data = new MeshDataProxy();
	pso = new PipelineStateObject();

	shader = shaderSystem.shaderMap.find(shaderFilename)->second;
}

void MeshComponent::Tick(double deltaTime)
{
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
	}
	else
	{
		pso->vertexBuffer = psoIt->second->vertexBuffer;
		pso->indexBuffer = psoIt->second->indexBuffer;
	}

	//Create texture and sampler
	if (!textureFilename.empty())
	{
		pso->texture = renderer.CreateTexture(textureFilename.c_str());
		pso->sampler = renderer.CreateSampler();
	}

	pso->rastState = renderer.rastStateMap["solid"];

	existingPiplineStateObjects[filename] = pso;
}

Properties MeshComponent::GetProps()
{
	Properties props("MeshComponent");
	props.Add("Filename", &filename);
	return props;
}
