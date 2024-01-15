#include "vpch.h"
#include "LockedDoor.h"

void LockedDoor::Activate()
{
	__super::Activate();

	SetActive(false);
	SetVisibility(false);
}

void LockedDoor::Deactivate()
{
	__super::Activate();

	SetActive(true);
	SetVisibility(true);
}
