#include "vpch.h"
#include "WaterVolumeValve.h"
#include "WaterVolume.h"

void WaterVolumeValve::Create()
{
	__super::Create();

	SetMeshFilename("gear.vmesh");
}

void WaterVolumeValve::Start()
{
	__super::Start();

	waterVolume = World::GetActorByNameAndLogCast<WaterVolume>(waterVolumeName);
	if (waterVolume)
	{
		originalWaterVolumeYPoint = waterVolume->GetYPointToRaiseTo();
		SetPlayerFocusGridActor(waterVolume);
	}
}

Properties WaterVolumeValve::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Water Volume", &waterVolumeName);
	props.Add("WaterVolume YPoint", &waterVolumeYRaisePoint);
	return props;
}

void WaterVolumeValve::OnLinkRotateRight()
{
	__super::OnLinkRotate();

	if (waterVolume)
	{
		waterVolume->SetYPointToRaiseTo(waterVolumeYRaisePoint);
	}
}

void WaterVolumeValve::OnLinkRotateLeft()
{
	__super::OnLinkRotateRight();

	if (waterVolume)
	{
		waterVolume->SetYPointToRaiseTo(originalWaterVolumeYPoint);
	}
}
