#include "vpch.h"
#include "PowerCrystalGear.h"

void PowerCrystalGear::Create()
{
	SetMeshFilename("gear.vmesh");
}

void PowerCrystalGear::PowerUp()
{
	AddRotation(GetUpVectorV(), 2.5f);
}
