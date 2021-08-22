#include "InstanceMeshComponent.h"
#include "Render/Renderer.h"

InstanceMeshComponent::InstanceMeshComponent(
	uint32_t meshInstanceRenderCount_, const char* filename, const wchar_t* shader)
	: MeshComponent(filename, shader)
{
	meshInstanceRenderCount = meshInstanceRenderCount_;
}

InstanceMeshComponent::~InstanceMeshComponent()
{
	structuredBuffer->Release();
	srv->Release();
}

void InstanceMeshComponent::Create()
{
	MeshComponent::Create();

	//Populate instance data
	instanceData.reserve(meshInstanceRenderCount);

	for (uint32_t i = 0; i < meshInstanceRenderCount; i++)
	{
		instanceData.push_back(InstanceData());
	}

	//Setup shader buffers
	structuredBuffer = renderer.CreateStructuredBuffer(sizeof(InstanceData) * instanceData.size(),
		sizeof(InstanceData), instanceData.data());
	assert(structuredBuffer);

	srv = renderer.CreateSRVForMeshInstance(structuredBuffer);
	assert(srv);
}
