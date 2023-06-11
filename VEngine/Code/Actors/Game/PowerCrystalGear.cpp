#include "vpch.h"
#include "PowerCrystalGear.h"
#include "Components/MeshComponent.h"

void PowerCrystalGear::Create()
{
	mesh->SetMeshFilename("gear.vmesh");
}

void PowerCrystalGear::PowerUp()
{
	AddRotation(GetUpVectorV(), 2.5f);
}
