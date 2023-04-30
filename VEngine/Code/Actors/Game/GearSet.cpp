#include "vpch.h"
#include "GearSet.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"

GearSet::GearSet()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void GearSet::Create()
{
	mesh->SetMeshFilename("gear.vmesh");

	canBeMovedInLink = false;
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
