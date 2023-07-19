#include "vpch.h"
#include "Bolt.h"

void Bolt::Create()
{
	SetMeshFilename("bolt.vmesh");
}

Properties Bolt::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
