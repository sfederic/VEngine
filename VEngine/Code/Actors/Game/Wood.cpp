#include "vpch.h"
#include "Wood.h"
#include "Components/MeshComponent.h"

void Wood::Create()
{
	canBePickedUp = false;
	itemName = "Wood";

	SetMeshFilename("wood.vmesh");
}

void Wood::Slice()
{
	//Make wood collectible only when it has been cut
	canBePickedUp = true;

	hasBeenCut = true;

	mesh->SetMeshFilename("cut_wood.vmesh");
	mesh->ReCreate();
}

void Wood::Interact()
{
	if (hasBeenCut && canBePickedUp)
	{
		__super::Interact();
	}
}
