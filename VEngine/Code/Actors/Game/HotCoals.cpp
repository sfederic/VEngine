#include "vpch.h"
#include "HotCoals.h"
#include "Components/MeshComponent.h"

void HotCoals::Create()
{
	mesh->SetMeshFilename("node.vmesh");
	mesh->SetTexture("hot_coals.jpg");
}

void HotCoals::Douse()
{
	Destroy();
}
