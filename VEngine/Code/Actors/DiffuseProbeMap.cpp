#include "vpch.h"
#include "DiffuseProbeMap.h"
#include <filesystem>
#include <algorithm>
#include "Core/World.h"
#include "Core/Log.h"
#include "Components/MeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/ShaderData/InstanceData.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	SetEmptyRootComponent();
}

void DiffuseProbeMap::Create()
{
	ReadProbeDataFromFile();

	const auto probeCount = GetProbeCount();

	lightProbesDebugInstanceMesh = new DebugLightProbe(probeCount);

	RenderUtils::CreateStructuredBuffer(sizeof(LightProbeInstanceData) * probeCount,
		sizeof(LightProbeInstanceData), lightProbeData.data(), structuredBuffer);
	RenderUtils::CreateSRVForMeshInstance(structuredBuffer.Get(), probeCount, srv);
}

void DiffuseProbeMap::PostCreate()
{
	__super::PostCreate();

	AssignStaticMeshesLightProbeIndex();
}

Properties DiffuseProbeMap::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Size X", &sizeX);
	props.Add("Size Y", &sizeY);
	props.Add("Size Z", &sizeZ);
	return props;
}

void DiffuseProbeMap::SetLightProbeData()
{
	int probeIndex = 0;

	const XMFLOAT3 pos = GetPosition();

	for (int x = pos.x; x < (sizeX + pos.x); x++)
	{
		for (int y = pos.y; y < (sizeY + pos.y); y++)
		{
			for (int z = pos.z; z < (sizeZ + pos.z); z++)
			{
				LightProbeInstanceData pd;
				pd.modelMatrix = XMMatrixTranslation((float)x, (float)y, (float)z);

				pd.modelMatrix.r[0].m128_f32[0] = 0.1f;
				pd.modelMatrix.r[1].m128_f32[1] = 0.1f;
				pd.modelMatrix.r[2].m128_f32[2] = 0.1f;

				XMStoreFloat3(&pd.position, pd.modelMatrix.r[3]);

				pd.index = probeIndex;

				lightProbeData.emplace_back(pd);

				probeIndex++;
			}
		}
	}
}

uint32_t DiffuseProbeMap::GetProbeCount()
{
	return sizeX * sizeY * sizeZ;
}

LightProbeInstanceData DiffuseProbeMap::GetProbeByIndex(int index)
{
	return lightProbeData.at(index);
}

LightProbeInstanceData DiffuseProbeMap::FindClosestProbe(XMVECTOR pos)
{
	struct ProbeDist
	{
		LightProbeInstanceData data;
		float distance = 0.f;
	};

	std::vector<ProbeDist> probeDistances;

	for (auto& probe : lightProbeData)
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
	size_t probeDataCount = lightProbeData.size();
	fwrite(&probeDataCount, sizeof(probeDataCount), 1, file);

	fwrite(lightProbeData.data(), sizeof(LightProbeInstanceData), probeDataCount, file);

	fclose(file);

	Log("[%s] binary probe data file created.", filename.c_str());
}

void DiffuseProbeMap::DeleteLightProbeDataFile()
{
	const std::string filename = GetWorldNameAsFilename();
	if (std::filesystem::exists(filename))
	{
		std::filesystem::remove(filename);
		Log("Deleted light probe file [%s].", filename.c_str());
	}
	else
	{
		Log("No [%s] light probe file to delete.", filename.c_str());
	}
}

void DiffuseProbeMap::ReadProbeDataFromFile()
{
	std::string filename = GetWorldNameAsFilename();
	if (std::filesystem::exists(filename))
	{
		FILE* file = nullptr;
		fopen_s(&file, filename.c_str(), "rb");
		assert(file);

		size_t probeDataCount = 0;
		fread(&probeDataCount, sizeof(probeDataCount), 1, file);
		assert(probeDataCount > 0);

		lightProbeData.reserve(probeDataCount);

		std::vector<LightProbeInstanceData> instanceData;
		instanceData.reserve(probeDataCount);

		for (int i = 0; i < probeDataCount; i++)
		{
			LightProbeInstanceData pd;
			fread(&pd, sizeof(LightProbeInstanceData), 1, file);
			lightProbeData.emplace_back(pd);

			instanceData.emplace_back(pd);
		}

		fclose(file);
	}
	else
	{
		SetLightProbeData();
	}
}

std::string DiffuseProbeMap::GetWorldNameAsFilename()
{
	std::string worldName = VString::GetSubStringBeforeFoundOffset(World::Get().GetWorldFilename(), "."); //trim .vmap
	std::string filename = "LightProbeData/" + worldName + ".probedata";
	return filename;
}

void DiffuseProbeMap::AssignStaticMeshesLightProbeIndex()
{
	auto& meshes = MeshComponent::system.GetComponents();
	for (auto& mesh : meshes)
	{
		if (mesh->IsRenderStatic())
		{
			auto lightProbe = FindClosestProbe(mesh->GetWorldPositionV());
			mesh->cachedLightProbeMapIndex = lightProbe.index;
		}
	}
}
