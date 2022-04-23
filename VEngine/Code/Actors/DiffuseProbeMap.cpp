#include "DiffuseProbeMap.h"
#include "Components/InstanceMeshComponent.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	instanceMeshComponent = InstanceMeshComponent::system.Add(this, InstanceMeshComponent(1, "cube.fbx", "test.png"));
	instanceMeshComponent->SetShaderName("InstanceShader.hlsl");
	rootComponent = instanceMeshComponent;

}

void DiffuseProbeMap::Create()
{
	int meshCount = sizeX * sizeY * sizeZ;
	instanceMeshComponent->SetInstanceCount(meshCount);
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

				data.world.r[0].m128_f32[0] = 0.2f;
				data.world.r[1].m128_f32[1] = 0.2f;
				data.world.r[2].m128_f32[2] = 0.2f;

				instanceData.push_back(data);
			}
		}
	}

	instanceMeshComponent->SetInstanceData(instanceData);
}
