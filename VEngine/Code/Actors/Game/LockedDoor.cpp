#include "vpch.h"
#include "LockedDoor.h"

void LockedDoor::Activate()
{
	__super::Activate();

	SetActive(false);
	SetVisibility(false);
}
