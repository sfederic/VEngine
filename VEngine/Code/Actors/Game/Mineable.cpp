#include "vpch.h"
#include "Mineable.h"

void Mineable::Create()
{
	SetMeshFilename("mound.vmesh");
}

void Mineable::Mine()
{
	Destroy();
}
