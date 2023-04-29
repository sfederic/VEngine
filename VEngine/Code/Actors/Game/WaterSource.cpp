#include "vpch.h"
#include "WaterSource.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

WaterSource::WaterSource()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void WaterSource::Create()
{
	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetRastState(RastStates::noBackCull);
}

Properties WaterSource::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

bool WaterSource::Contains(XMVECTOR point)
{
	return boxTrigger->Contains(point);
}
