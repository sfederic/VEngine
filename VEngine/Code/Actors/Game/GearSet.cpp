#include "vpch.h"
#include "GearSet.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "GearSetDoor.h"
#include "Core/Log.h"

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
	props.Add("Door Name", &gearSetDoorName);
	return props;
}

bool GearSet::Intersects(const BoundingOrientedBox& intersectCheck) const
{
	return boxTrigger->Intersects(intersectCheck);
}

void GearSet::IncrementDoor()
{
	auto door = dynamic_cast<GearSetDoor*>(World::GetActorByNameAllowNull(gearSetDoorName));
	if (door)
	{
		if (numDoorIncrementsDone < door->maxNumIncrements)
		{
			numDoorIncrementsDone++;
			door->AddNextPosition(XMVectorSet(0.f, 0.1f, 0.f, 1.f));
		}
	}
	else
	{
		Log("No GearSetDoor with name [%s] found.", gearSetDoorName.c_str());
	}
}
