#include "vpch.h"
#include "WaterVolume.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/VMath.h"

WaterVolume::WaterVolume()
{
	waterSurface = CreateComponent<MeshComponent>("WaterSurfaceMesh");
	rootComponent = waterSurface;

	waterVolumeTrigger = CreateComponent<BoxTriggerComponent>("VolumeTrigger");
	rootComponent->AddChild(waterVolumeTrigger);
}

void WaterVolume::Create()
{
	waterSurface->ignoreGridRaycasts = true;
	waterSurface->SetMeshFilename("node.vmesh");
	waterSurface->SetTexture("water.jpg");

	//Set just below the mesh representing the surface
	waterVolumeTrigger->SetLocalPosition(0.f, -0.5f, 0.f);
}

void WaterVolume::Tick(float deltaTime)
{
	//@Todo: Testing code for raising water levels
	auto targetPos = GetPositionV();
	targetPos.m128_f32[1] = yPointToRaiseTo;
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), targetPos, deltaTime, 1.0f));
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
