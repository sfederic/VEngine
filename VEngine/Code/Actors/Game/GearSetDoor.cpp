#include "vpch.h"
#include "GearSetDoor.h"
#include "Components/MeshComponent.h"

void GearSetDoor::Create()
{
	canFall = false;
	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	mesh->SetMeshFilename("plane.vmesh");
}
