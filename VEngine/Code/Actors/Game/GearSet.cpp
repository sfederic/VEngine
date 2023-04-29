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

void GearSet::Create()
{
	mesh->SetMeshFilename("gear.vmesh");
}

Properties GearSet::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

bool GearSet::Intersects(const BoundingOrientedBox& intersectCheck) const
{
	return boxTrigger->Intersects(intersectCheck);
}
