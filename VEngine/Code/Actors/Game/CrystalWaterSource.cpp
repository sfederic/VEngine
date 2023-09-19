#include "vpch.h"
#include "CrystalWaterSource.h"
#include "WaterSource.h"

void CrystalWaterSource::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");
}

void CrystalWaterSource::Start()
{
	__super::Start();

	waterSource = World::GetActorByNameAndLogCast<WaterSource>(waterSourceName);
}

Properties CrystalWaterSource::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Water Source", &waterSourceName);
	return props;
}

void CrystalWaterSource::Activate()
{
	waterSource->Activate();
}

void CrystalWaterSource::Deactivate()
{
	waterSource->Deactivate();
}
