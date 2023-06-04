#include "vpch.h"
#include "WaterValve.h"
#include "Core/Log.h"
#include "Components/MeshComponent.h"
#include "WaterSource.h"

void WaterValve::Create()
{
	mesh->SetMeshFilename("gear.vmesh");
}

Properties WaterValve::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Water Source", &waterSourceName);
	return props;
}

void WaterValve::OnLinkRotate()
{
	auto waterSource = dynamic_cast<WaterSource*>(World::GetActorByNameAllowNull(waterSourceName));
	if (waterSource)
	{
		waterSource->ToggleActive();
		waterSource->ToggleVisibility();
	}
	else
	{
		Log("Water Source [%s] not found.", waterSourceName.c_str());
	}
}
