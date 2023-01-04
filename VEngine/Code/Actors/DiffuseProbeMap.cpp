#include "vpch.h"
#include "DiffuseProbeMap.h"
#include <filesystem>
#include "Components/InstanceMeshComponent.h"
#include "Debug.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	//Set mesh count as 1
	instanceMeshComponent = InstanceMeshComponent::system.Add(
		"InstanceMesh",
		this,
		InstanceMeshComponent(1, "cube.fbx", "test.png", ShaderItems::Instance));
	rootComponent = instanceMeshComponent;
}

void DiffuseProbeMap::Create()
{
	instanceMeshComponent->SetInstanceCount(GetProbeCount());
	SetInstanceMeshData();
	ReadProbeDataFromFile();
}

Properties DiffuseProbeMap::GetProps()
{
	auto props = __super::GetProps();
	props.title = "DiffuseProbeMap";
	props.Add("Size X", &sizeX);
	props.Add("Size Y", &sizeY);
	props.Add("Size Z", &sizeZ);
	return props;
}

void DiffuseProbeMap::SetInstanceMeshData()
{
	std::vector<InstanceData> instanceData;

	int probeIndex = 0;

	XMFLOAT3 pos = GetPosition();

	for (int x = pos.x; x < (sizeX + pos.x); x++)
	{
		for (int y = pos.y; y < (sizeY + pos.y); y++)
		{
			for (int z = pos.z; z < (sizeZ + pos.z); z++)
			{
				InstanceData data;
				data.world = XMMatrixTranslation((float)x, (float)y, (float)z);
				
				data.world.r[0].m128_f32[0] = 0.15f;
				data.world.r[1].m128_f32[1] = 0.15f;
				data.world.r[2].m128_f32[2] = 0.15f;

				instanceData.emplace_back(data);

				ProbeData pd;
				pd.index = probeIndex;
				XMStoreFloat3(&pd.position,data.world.r[3]);
				probeData.emplace_back(pd);

				probeIndex++;
			}
		}
	}

	instanceMeshComponent->SetInstanceData(instanceData);
}

void DiffuseProbeMap::SetProbeColour(XMFLOAT3 colour, uint32_t instanceMeshIndex)
{
}

uint32_t DiffuseProbeMap::GetProbeCount()
{
	return sizeX * sizeY * sizeZ;
}

ProbeData DiffuseProbeMap::FindClosestProbe(XMVECTOR pos)
{
	std::map<float, ProbeData> distanceMap;

	for (auto& probe : probeData)
	{
		distanceMap[XMVector3Length(XMLoadFloat3(&probe.position) - pos).m128_f32[0]] = probe;
	}

	if (!distanceMap.empty())
	{
		return distanceMap.begin()->second;
	}

	throw new std::exception("No probe found");
}

void DiffuseProbeMap::WriteProbeDataToFile()
{
	const std::string filename = "probemap.probedata";
	FILE* file = nullptr;
	fopen_s(&file, filename.c_str(), "wb");
	assert(file);

	//Write probe data count to read back.
	int probeDataCount = probeData.size();
	fwrite(&probeDataCount, sizeof(int), 1, file);

	fwrite(probeData.data(), sizeof(ProbeData), probeDataCount, file);

	fclose(file);

	Log("[%s] binary probe data file created.", filename.c_str());
}

void DiffuseProbeMap::ReadProbeDataFromFile()
{
	const std::string filename = "probemap.probedata";
	if (std::filesystem::exists(filename))
	{
		FILE* file = nullptr;
		fopen_s(&file, filename.c_str(), "rb");
		assert(file);

		int probeDataCount = 0;
		fread(&probeDataCount, sizeof(int), 1, file);
		assert(probeDataCount > 0);

		fread(probeData.data(), sizeof(ProbeData), probeDataCount, file);

		fclose(file);
	}
}
