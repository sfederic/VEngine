
#include "GearSetDoor.h"

void GearSetDoor::Create()
{
	__super::Create();

	canFall = false;
	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	SetMeshFilename("plane.vmesh");
}

Properties GearSetDoor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
