#include "InstanceMeshComponent.h"
#include "Render/RenderUtils.h"

InstanceMeshComponent::InstanceMeshComponent(
	uint32_t meshInstanceRenderCount_, const std::string filename, const std::string textureFilename)
	: MeshComponent(filename, textureFilename)
{
	meshInstanceRenderCount = meshInstanceRenderCount_;
}

InstanceMeshComponent::~InstanceMeshComponent()
{
	if(structuredBuffer) structuredBuffer->Release();
	if(srv) srv->Release();
}

void InstanceMeshComponent::Create()
{
	MeshComponent::Create();

	//Populate instance data
	instanceData.reserve(meshInstanceRenderCount);

	for (uint32_t i = 0; i < meshInstanceRenderCount; i++)
	{
		InstanceData data = {};
		data.world = XMMatrixIdentity();
		instanceData.push_back(data);
	}

	//Setup shader buffers
	structuredBuffer = RenderUtils::CreateStructuredBuffer(sizeof(InstanceData) * meshInstanceRenderCount,
		sizeof(InstanceData), instanceData.data());
	assert(structuredBuffer);

	srv = RenderUtils::CreateSRVForMeshInstance(structuredBuffer, meshInstanceRenderCount);
	assert(srv);
}

void InstanceMeshComponent::SetInstanceData(std::vector<InstanceData>& instanceData_)
{
	instanceData.clear();
	instanceData = instanceData_;
}

Properties InstanceMeshComponent::GetProps()
{
	Properties props = MeshComponent::GetProps();
	props.title = "InstanceMeshComponent";
	return props;
}
