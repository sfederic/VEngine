#include "vpch.h"
#include "WaterVolumeValve.h"
#include "WaterVolume.h"

void WaterVolumeValve::Create()
{
	__super::Create();

	SetMeshFilename("gear.vmesh");
}

Properties WaterVolumeValve::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Water Volume", &waterVolumeName);
	props.Add("WaterVolume YPoint", &waterVolumeYRaisePoint);
	return props;
}

void WaterVolumeValve::OnLinkRotate()
{
	__super::OnLinkRotate();

	auto waterVolume = World::GetActorByNameAndLogCast<WaterVolume>(waterVolumeName);
	if (waterVolume)
	{
		waterVolume->SetYPointToRaiseWaterTo(waterVolumeYRaisePoint);
	}
}
