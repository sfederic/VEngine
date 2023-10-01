#include "vpch.h"
#include "WorldResetter.h"
#include "Core/FileSystem.h"

void WorldResetter::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");
}

void WorldResetter::Interact()
{
	FileSystem::SetDeferredWorldReset();
}
