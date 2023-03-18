#include "vpch.h"
#include "DiffuseProbeMap.h"
#include <filesystem>
#include <algorithm>
#include "Core/Debug.h"
#include "Core/World.h"
#include "Core/Log.h"
#include "Components/InstanceMeshComponent.h"
#include "Render/Material.h"
#include "Render/ShaderData/InstanceData.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	//Set mesh count as 1
	instanceMeshComponent = InstanceMeshComponent::system.Add(
		"InstanceMesh",
		this,
		InstanceMeshComponent(1, "cube.vmesh", "test.png", ShaderItems::Instance));
	rootComponent = instanceMeshComponent;
}

void DiffuseProbeMap::Create()
{
	instanceMeshComponent->GetMaterial().materialShaderData.useTexture = 0;
	instanceMeshComponent->SetInstanceCount(GetProbeCount());

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
				
				data.world.r[0].m128_f32[0] = 0.05f;
				data.world.r[1].m128_f32[1] = 0.05f;
				data.world.r[2].m128_f32[2] = 0.05f;

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
	struct ProbeDist
	{
		ProbeData data;
		float distance = 0.f;
	};

	std::vector<ProbeDist> probeDistances;

	for (auto& probe : probeData)
	{
		const float distance = XMVector3Length(XMLoadFloat3(&probe.position) - pos).m128_f32[0];
		ProbeDist probeDist;
		probeDist.distance = distance;
		probeDist.data = probe;
		probeDistances.emplace_back(probeDist);
	}

	auto ProbeDistCompare = [](const ProbeDist& l, const ProbeDist& r) -> bool {
		return l.distance < r.distance;
	};

	std::sort(probeDistances.begin(), probeDistances.end(), ProbeDistCompare);

	return probeDistances.front().data;
}

void DiffuseProbeMap::WriteProbeDataToFile()
{
	std::string filename = GetWorldNameAsFilename();
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
	std::string filename = GetWorldNameAsFilename();
	if (std::filesystem::exists(filename))
	{
		FILE* file = nullptr;
		fopen_s(&file, filename.c_str(), "rb");
		assert(file);

		int probeDataCount = 0;
		fread(&probeDataCount, sizeof(int), 1, file);
		assert(probeDataCount > 0);

		probeData.reserve(probeDataCount);

		std::vector<InstanceData> instanceData;
		instanceData.reserve(probeDataCount);

		for (int i = 0; i < probeDataCount; i++)
		{
			ProbeData pb;
			fread(&pb, sizeof(ProbeData), 1, file);

			probeData.emplace_back(pb);

			InstanceData data;
			data.world = XMMatrixTranslation(pb.position.x, pb.position.y, pb.position.z);
			data.world.r[0].m128_f32[0] = 0.15f;
			data.world.r[1].m128_f32[1] = 0.15f;
			data.world.r[2].m128_f32[2] = 0.15f;

			SetProbeVisualColourFromIrradiance(pb, data);

			instanceData.emplace_back(data);
		}

		fclose(file);

		instanceMeshComponent->SetInstanceData(instanceData);
	}
	else
	{
		SetInstanceMeshData();
	}
}

void DiffuseProbeMap::SetProbeVisualColourFromIrradiance(ProbeData& pb, InstanceData& data)
{
	constexpr float PI = 3.14159265f;

	constexpr float SQRT_PI = 1.7724538509f;
	constexpr float SQRT_5 = 2.2360679775f;
	constexpr float SQRT_15 = 3.8729833462f;
	constexpr float SQRT_3 = 1.7320508076f;

	constexpr float AO = 0.75f;

	constexpr float Y[9] =
	{
		1.0f / (2.0f * SQRT_PI),
		-SQRT_3 / (2.0f * SQRT_PI),
		SQRT_3 / (2.0f * SQRT_PI),
		-SQRT_3 / (2.0f * SQRT_PI),
		SQRT_15 / (2.0f * SQRT_PI),
		-SQRT_15 / (2.0f * SQRT_PI),
		SQRT_5 / (4.0f * SQRT_PI),
		-SQRT_15 / (2.0f * SQRT_PI),
		SQRT_15 / (4.0f * SQRT_PI)
	};

	float t = acos(sqrt(1 - AO));

	float a = sin(t);
	float b = cos(t);

	float A0 = sqrt(4 * PI) * (sqrt(PI) / 2) * a * a;
	float A1 = sqrt(4 * PI / 3) * (sqrt(3 * PI) / 3) * (1 - b * b * b);
	float A2 = sqrt(4 * PI / 5) * (sqrt(5 * PI) / 16) * a * a * (2 + 6 * b * b);

	XMFLOAT3 n = XMFLOAT3(0.f, 1.f, 0.f); //Essentially put in a random normal
	XMStoreFloat3(&n, XMVector3Normalize(data.world.r[3]));

	XMFLOAT4 coefs[9]{};
	memcpy(&coefs[0], &pb.SH[0], sizeof(XMFLOAT4) * 9);

	XMVECTOR irradiance =
		XMLoadFloat4(&coefs[0]) * A0 * Y[0] +
		XMLoadFloat4(&coefs[1]) * A1 * Y[1] * n.y +
		XMLoadFloat4(&coefs[2]) * A1 * Y[2] * n.z +
		XMLoadFloat4(&coefs[3]) * A1 * Y[3] * n.x +
		XMLoadFloat4(&coefs[4]) * A2 * Y[4] * (n.y * n.x) +
		XMLoadFloat4(&coefs[5]) * A2 * Y[5] * (n.y * n.z) +
		XMLoadFloat4(&coefs[6]) * A2 * Y[6] * (3.0 * n.z * n.z - 1.0) +
		XMLoadFloat4(&coefs[7]) * A2 * Y[7] * (n.z * n.x) +
		XMLoadFloat4(&coefs[8]) * A2 * Y[8] * (n.x * n.x - n.y * n.y);

	irradiance = DirectX::XMVectorMax(irradiance, XMVectorZero());
	irradiance.m128_f32[3] = 1.0f; //Make sure alpha is set
	XMStoreFloat4(&data.colour, irradiance);
}

std::string DiffuseProbeMap::GetWorldNameAsFilename()
{
	std::string worldName = VString::GetSubStringBeforeFoundOffset(World::worldFilename, "."); //trimp .vmap
	std::string filename = "LightProbeData/" + worldName + ".probedata";
	return filename;
}
