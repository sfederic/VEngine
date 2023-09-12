#include "vpch.h"
#include "PowerCrystalGear.h"

void PowerCrystalGear::Create()
{
	__super::Create();
	SetMeshFilename("gear.vmesh");
}

void PowerCrystalGear::PowerUp()
{
	__super::PowerUp();

	AddRotation(GetUpVectorV(), 2.5f);
}
