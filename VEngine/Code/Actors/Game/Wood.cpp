#include "vpch.h"
#include "Wood.h"
#include "Components/MeshComponent.h"

void Wood::Create()
{
	SetMeshFilename("wood.vmesh");
}

void Wood::Slice()
{
	hasBeenCut = true;

	mesh->SetMeshFilename("cut_wood.vmesh");
	mesh->ReCreate();
}

void Wood::Interact()
{
	if (hasBeenCut)
	{
		//@Todo: set to an "inventory"
		RecalcCurrentNodePosition();
		Destroy();
	}
}
