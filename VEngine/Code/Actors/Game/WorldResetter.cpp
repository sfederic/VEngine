#include "vpch.h"
#include "WorldResetter.h"
#include "Core/FileSystem.h"

void WorldResetter::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");
}

Properties WorldResetter::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void WorldResetter::Interact()
{
	FileSystem::SetDeferredWorldReset();
}
