#include "vpch.h"
#include "Mineable.h"
#include "Components/MeshComponent.h"

void Mineable::Create()
{
	mesh->SetMeshFilename("mound.vmesh");
}

void Mineable::Mine()
{
	Destroy();
}
