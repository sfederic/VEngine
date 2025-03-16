
#include "Mineable.h"

void Mineable::Create()
{
	__super::Create();

	SetMeshFilename("mound.vmesh");
}

void Mineable::Mine()
{
	Remove();
}
