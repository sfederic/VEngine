#include "vpch.h"
#include "WaterVolume.h"
#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Render/BlendStates.h"

WaterVolume::WaterVolume()
{
	SetEmptyRootComponent();

	waterMesh = CreateComponent<MeshComponent>("WaterMesh");
	AddChildToRoot(waterMesh);

	waterVolumeTrigger = CreateComponent<BoxTriggerComponent>("VolumeTrigger");
	AddChildToRoot(waterVolumeTrigger);
}

void WaterVolume::Create()
{
	__super::Create();

	waterMesh->ignoreGridRaycasts = true;
	waterMesh->SetMeshFilename("cube.vmesh");
	waterMesh->SetTexture("water.jpg");
	waterMesh->SetBlendState(BlendStates::Transparent);
}

void WaterVolume::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	DouseGridActorsInWaterVolume();
}

Properties WaterVolume::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

bool WaterVolume::Contains(XMVECTOR point)
{
	return waterVolumeTrigger->Contains(point);
}

std::vector<GridActor*> WaterVolume::GetAllGridActorsWithinVolume()
{
	std::vector<GridActor*> outActors;
	for (auto gridActor : World::Get().GetAllActorsAsBaseType<GridActor>())
	{
		if (Contains(gridActor->GetPositionV()))
		{
			outActors.emplace_back(gridActor);
		}
	}
	return outActors;
}

void WaterVolume::DouseGridActorsInWaterVolume()
{
	for (auto gridActor : World::Get().GetAllActorsAsBaseType<GridActor>())
	{
		if (Contains(gridActor->GetPositionV()))
		{
			gridActor->Douse();
		}
	}
}
