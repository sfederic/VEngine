#include "vpch.h"
#include "InstanceMeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/ShaderData/InstanceData.h"

InstanceMeshComponent::InstanceMeshComponent(uint32_t meshInstanceRenderCount_,
	const std::string filename,
	const std::string textureFilename,
	ShaderItem* shaderItem)
	: MeshComponent(filename, textureFilename, shaderItem)
{
	meshInstanceRenderCount = meshInstanceRenderCount_;

	instanceData.resize(meshInstanceRenderCount);
}

void InstanceMeshComponent::Create()
{
	MeshComponent::Create();

	if (instanceData.empty())
	{
		instanceData.emplace_back(InstanceData());
	}

	uint32_t meshCount = meshInstanceRenderCount;

	//Set a default of 1 if 0 so that the SRV buffers don't blow up when creating them.
	if (meshCount == 0)
	{
		meshCount = 1;
	}

	//Setup shader buffers
	RenderUtils::CreateStructuredBuffer(sizeof(InstanceData) * meshCount,
		sizeof(InstanceData), instanceData.data(), structuredBuffer);

	RenderUtils::CreateSRVForMeshInstance(structuredBuffer.Get(), meshCount, srv);
}

void InstanceMeshComponent::SetInstanceCount(uint32_t count)
{
	meshInstanceRenderCount = count;
}

uint32_t InstanceMeshComponent::GetInstanceCount()
{
	return meshInstanceRenderCount;
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

void InstanceMeshComponent::ReleaseBuffers()
{
	if (structuredBuffer)
	{
		structuredBuffer.Reset();
	}
	if (srv)
	{
		srv.Reset();
	}
}
