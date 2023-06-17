#include "vpch.h"
#include "WaterSource.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

WaterSource::WaterSource()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void WaterSource::Create()
{
	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetRastState(RastStates::noBackCull);
	mesh->SetTexture("water.jpg");
	mesh->SetUVOffsetSpeed(XMFLOAT2(0.f, 0.25f));
}

void WaterSource::Start()
{
	__super::Start();

	SetVisibility(visible);
}

Properties WaterSource::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void WaterSource::Activate()
{
	SetVisibility(true);
}

void WaterSource::Deactivate()
{
	SetVisibility(false);
}

bool WaterSource::Contains(XMVECTOR point)
{
	return boxTrigger->Contains(point);
}
