#include "vpch.h"
#include "DoorSwitch.h"
#include "Actors/Game/Door.h"

Properties DoorSwitch::GetProps()
{
	Properties props = __super::GetProps();
	props.Add("Door Name", &linkedDoorName);
	return props;
}

void DoorSwitch::Interact()
{
	Actor* linkedDoor = World::GetActorByNameAllowNull(linkedDoorName);
	if (linkedDoor)
	{
		Door* door = dynamic_cast<Door*>(linkedDoor);
		if (door)
		{
			door->Open();
		}

		return;
	}

	Log("[%s] door not found on Interact for [%s]", linkedDoorName.c_str(), GetName().c_str());
}
