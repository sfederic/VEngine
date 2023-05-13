#include "vpch.h"
#include "WaterVolume.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"

WaterVolume::WaterVolume()
{
	waterSurface = CreateComponent<MeshComponent>("WaterSurfaceMesh");
	rootComponent = waterSurface;

	waterVolumeTrigger = CreateComponent<BoxTriggerComponent>("VolumeTrigger");
	rootComponent->AddChild(waterVolumeTrigger);
}

void WaterVolume::Create()
{
	waterSurface->gridObstacle = true;
	waterSurface->SetMeshFilename("node.vmesh");
	waterSurface->SetTexture("water.jpg");

	//Set just below the mesh representing the surface
	waterVolumeTrigger->SetLocalPosition(0.f, -0.5f, 0.f);
}

Properties WaterVolume::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
