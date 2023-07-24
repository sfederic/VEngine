#include "vpch.h"
#include "Wood.h"
#include "Components/MeshComponent.h"

void Wood::Create()
{
	SetMeshFilename("wood.vmesh");
}

void Wood::Slice()
{
	mesh->SetMeshFilename("cut_wood.vmesh");
	mesh->ReCreate();
}
