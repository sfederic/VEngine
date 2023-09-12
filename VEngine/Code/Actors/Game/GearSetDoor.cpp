#include "vpch.h"
#include "GearSetDoor.h"

void GearSetDoor::Create()
{
	__super::Create();

	canFall = false;
	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	SetMeshFilename("plane.vmesh");
}
