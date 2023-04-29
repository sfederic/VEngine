#include "vpch.h"
#include "GearSet.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"

GearSet::GearSet()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

Properties GearSet::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

bool GearSet::Contains(XMVECTOR point)
{
	return boxTrigger->Contains(point);
}
