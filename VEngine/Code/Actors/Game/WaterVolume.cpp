#include "vpch.h"
#include "WaterVolume.h"
#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/VMath.h"

WaterVolume::WaterVolume()
{
	SetEmptyRootComponent();

	waterSurface = CreateComponent<MeshComponent>("WaterSurfaceMesh");
	rootComponent->AddChild(waterSurface);

	waterVolumeTrigger = CreateComponent<BoxTriggerComponent>("VolumeTrigger");
	rootComponent->AddChild(waterVolumeTrigger);
}

void WaterVolume::Create()
{
	__super::Create();

	waterSurface->ignoreGridRaycasts = true;
	waterSurface->SetMeshFilename("node.vmesh");
	waterSurface->SetTexture("water.jpg");

	//Set just below the mesh representing the surface
	waterVolumeTrigger->SetLocalPosition(0.f, -0.5f, 0.f);
}

void WaterVolume::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	//@Todo: Testing code for raising water levels
	auto targetPos = GetPositionV();
	targetPos.m128_f32[1] = yPointToRaiseTo;
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), targetPos, deltaTime, 1.0f));

	DouseGridActorsInWaterVolume();
}

Properties WaterVolume::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Raise Y Point", &yPointToRaiseTo);
	return props;
}

bool WaterVolume::Contains(XMVECTOR point)
{
	return waterVolumeTrigger->Contains(point);
}

std::vector<GridActor*> WaterVolume::GetAllGridActorsWithinVolume()
{
	std::vector<GridActor*> outActors;
	for (auto gridActor : World::GetAllActorsAsBaseType<GridActor>())
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
	for (auto gridActor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (Contains(gridActor->GetPositionV()))
		{
			gridActor->Douse();
		}
	}
}
