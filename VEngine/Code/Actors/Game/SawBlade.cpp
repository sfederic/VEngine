#include "vpch.h"
#include "SawBlade.h"

void SawBlade::Create()
{
	SetMeshFilename("saw_blade.vmesh");
}

void SawBlade::PowerUp()
{
	isCutting = true;
	AddRotation(GetUpVectorV(), 50.5f);
}

void SawBlade::PowerDown()
{
	isCutting = false;
}
