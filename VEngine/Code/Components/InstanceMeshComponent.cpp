#include "vpch.h"
#include "InstanceMeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/Material.h"
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

InstanceMeshComponent::~InstanceMeshComponent()
{
	if (structuredBuffer) structuredBuffer->Release();
	if (srv) srv->Release();
}

void InstanceMeshComponent::Create()
{
	MeshComponent::Create();

	if (instanceData.empty())
	{
		instanceData.emplace_back(InstanceData());
	}

	//Setup shader buffers
	structuredBuffer = RenderUtils::CreateStructuredBuffer(sizeof(InstanceData) * meshInstanceRenderCount,
		sizeof(InstanceData), instanceData.data());

	srv = RenderUtils::CreateSRVForMeshInstance(structuredBuffer, meshInstanceRenderCount);
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
		structuredBuffer->Release();
	}
	if (srv)
	{
		srv->Release();
	}
}
