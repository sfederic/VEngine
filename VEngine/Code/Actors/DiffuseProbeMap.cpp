#include "DiffuseProbeMap.h"
#include "Components/InstanceMeshComponent.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	int meshCount = sizeX * sizeY * sizeZ;
	instanceMeshComponent = InstanceMeshComponent::system.Add(this, InstanceMeshComponent(meshCount, "cube.fbx", "test.png"));
	instanceMeshComponent->SetShaderName("InstanceShader.hlsl");
	rootComponent = instanceMeshComponent;

	SetInstanceMeshData();
}

Properties DiffuseProbeMap::GetProps()
{
	auto props = __super::GetProps();
	props.title = "DiffuseProbeMap";
	props.AddProp(sizeX);
	props.AddProp(sizeY);
	props.AddProp(sizeZ);
	return props;
}

void DiffuseProbeMap::SetInstanceMeshData()
{
	std::vector<InstanceData> instanceData;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			for (int z = 0; z < sizeZ; z++)
			{
				InstanceData data = {};
				data.world = XMMatrixTranslation((float)x, (float)y, (float)z);
				instanceData.push_back(data);
			}
		}
	}

	instanceMeshComponent->SetInstanceData(instanceData);
}
